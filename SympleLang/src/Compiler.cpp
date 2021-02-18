#include "SympleCode/Compiler.h"

#include <sstream>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Parser.h"
#include "SympleCode/Binding/Binder.h"
#include "SympleCode/Emit/AsmEmitter.h"
#include "SympleCode/Util/ConsoleColor.h"

namespace Symple
{
	std::vector<std::string> Compiler::sLibraries;

	Compiler::Compiler(char *path)
		: mPath(path)
	{ mAsmPath = mPath.substr(0, mPath.find_first_of('/')) + "/bin" + mPath.substr(mPath.find_first_of('/'), mPath.find_last_of('.') - mPath.find_first_of('/')) + ".S"; }

	shared_ptr<DiagnosticBag> Compiler::Lex()
	{
		if (mAnyErrors)
			return nullptr;

		unique_ptr<Syntax::Lexer> lexer = make_unique<Syntax::Lexer>((char*)mPath.c_str());
		mTokens.clear();

		do
			mTokens.push_back(lexer->Lex());
		while (!mTokens.back()->Is(Syntax::Token::EndOfFile));

		if (PrintDiagnosticBag(lexer->GetDiagnosticBag(), "Lexing"))
			return lexer->GetDiagnosticBag();

#if __SY_DEBUG
		std::stringstream ss;
		for (auto tok : mTokens)
		{
			tok->Print(ss, "", tok->Is(Syntax::Token::EndOfFile));
			ss.put('\n');
		}
		spdlog::debug("Lex Tokens:\n{}", ss.str());
#endif

		return lexer->GetDiagnosticBag();
	}

	shared_ptr<DiagnosticBag> Compiler::Parse()
	{
		if (mAnyErrors)
			return nullptr;

		unique_ptr<Syntax::Parser> parser = make_unique<Syntax::Parser>(mTokens);
		mAST = parser->Parse();
		if (PrintDiagnosticBag(parser->GetDiagnosticBag(), "Parsing"))
			return parser->GetDiagnosticBag();

#if __SY_DEBUG
		std::stringstream ss;
		mAST->Print(ss);
		ss.put('\n');
		spdlog::debug("Parse Tree:\n{}", ss.str());
#endif

		return parser->GetDiagnosticBag();
	}

	shared_ptr<DiagnosticBag> Compiler::Bind()
	{
		if (mAnyErrors)
			return nullptr;

		shared_ptr<Binding::Binder> binder = make_shared<Binding::Binder>();
		mTree = binder->Bind(mAST);
		if (PrintDiagnosticBag(binder->GetDiagnosticBag(), "Binding"))
			return binder->GetDiagnosticBag();

#if __SY_DEBUG
		std::stringstream ss;
		mTree->Print(ss);
		ss.put('\n');
		spdlog::info("Bound Tree:\n{}", ss.str());

		using namespace Symple::Util;
		ResetConsoleColor();
#endif

		return binder->GetDiagnosticBag();
	}

	shared_ptr<Binding::BoundCompilationUnit> Compiler::BindSymbols()
	{
		if (mAnyErrors)
			return nullptr;

		shared_ptr<Binding::Binder> binder = make_shared<Binding::Binder>();
		mTree = binder->BindSymbols(mAST);
		if (PrintDiagnosticBag(binder->GetDiagnosticBag(), "Importing"))
			return mTree;

#if __SY_DEBUG
		std::stringstream ss;
		mTree->Print(ss);
		spdlog::info("Bound Tree:\n{}", ss.str());
		ss.put('\n');

		using namespace Symple::Util;
		ResetConsoleColor();
#endif

		return mTree;
	}

	void Compiler::Emit()
	{
		if (mAnyErrors)
			return;

		mEmitter = make_unique<Emit::AsmEmitter>((char*)mAsmPath.c_str());
		mEmitter->Emit(mTree);
	}

	void Compiler::Compile()
	{
		if (mAnyErrors)
			return;

		mEmitter->Compile();
		mEmitter.release();
	}

	bool Compiler::Link()
	{
		if (mAnyErrors)
			return false;

		std::stringstream linkcmd;
		linkcmd << "clang -m32 --optimize -o sy/bin/Main.exe " << mAsmPath.substr(0, mAsmPath.find_last_of('.')) << ".obj";
		for (auto compiler : mUnits)
			linkcmd << ' ' << compiler->mAsmPath.substr(0, mAsmPath.find_last_of('.')) << ".obj";
		for (auto lib : sLibraries)
			linkcmd << " -l " << lib;

		return !system(linkcmd.str().c_str());
	}
	
	int Compiler::Exec(std::string_view args)
	{
		Util::SetConsoleColor(Util::Yellow);
		puts("Executing program...");
		Util::ResetConsoleColor();

		char* cmd = new char[17 + args.length()];
		snprintf(cmd, 17 + args.length(), "sy\\bin\\Main.exe %s", args.data());
		int ec = system(cmd);
		delete cmd;

		Util::SetConsoleColor(Util::Yellow);
		printf("\nProgram Exited with code %i (0x%x)", ec, ec);
		Util::ResetConsoleColor();

		return ec;
	}

	bool Compiler::PrintDiagnosticBag(shared_ptr<DiagnosticBag> diagnostics, char *step)
	{
		using namespace Symple::Util;
		SetConsoleColor(Yellow);

		for (shared_ptr<Diagnostic> diagnostic : diagnostics->GetDiagnostics())
			switch (diagnostic->GetLevel())
			{
			case Diagnostic::Message:
				spdlog::info("'{}' {}:{} \"{}\"", diagnostic->GetToken()->GetFile(), diagnostic->GetToken()->GetLine(), diagnostic->GetToken()->GetColumn(), diagnostic->GetMessage());
				break;
			case Diagnostic::Warning:
				spdlog::warn("'{}' {}:{} \"{}\"", diagnostic->GetToken()->GetFile(), diagnostic->GetToken()->GetLine(), diagnostic->GetToken()->GetColumn(), diagnostic->GetMessage());
				break;
			case Diagnostic::Error:
				spdlog::error("'{}' {}:{} \"{}\"", diagnostic->GetToken()->GetFile(), diagnostic->GetToken()->GetLine(), diagnostic->GetToken()->GetColumn(), diagnostic->GetMessage());
				break;
			}

		if (diagnostics->GetErrorCount())
		{
			spdlog::info("{} '{}' failed with {} errors, {} warnings, {} messages", step, mPath, diagnostics->GetErrorCount(), diagnostics->GetWarningCount(), diagnostics->GetMessageCount());

			return mAnyErrors = true;
		}
		else
		{
			spdlog::info("{} '{}' completed with {} errors, {} warnings, {} messages", step, mPath, diagnostics->GetErrorCount(), diagnostics->GetWarningCount(), diagnostics->GetMessageCount());

			return false;
		}
	}
}