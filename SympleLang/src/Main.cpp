#include <iostream>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Parser.h"
#include "SympleCode/Binding/Binder.h"
#include "SympleCode/Emit/AsmEmitter.h"

using namespace Symple;
using namespace Symple::Binding;
using namespace Symple::Syntax;
using namespace Symple::Emit;

using std::shared_ptr;
using std::make_shared;

using std::unique_ptr;
using std::make_unique;

using spdlog::level::level_enum;

#ifdef _WIN32
enum ConsoleColor
{
	Black,
	DarkBlue,
	DarkGreen,
	DarkCyan,
	DarkRed,
	DarkMagenta,
	DarkYellow,
	Grey,
	DarkGrey,
	Blue,
	Green,
	Cyan,
	Red,
	Magenta,
	Yellow,
	White,

	Reset = White,
};
#endif

bool PrintDiagnosticBag(shared_ptr<DiagnosticBag> diagnostics, char step[] = "Null Step")
{
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Yellow);
#endif

	unsigned errCount = 0, warningCount = 0, messageCount = 0;

	for (shared_ptr<Diagnostic> diagnostic : diagnostics->GetDiagnostics())
		switch (diagnostic->GetLevel())
		{
		case Diagnostic::Message:
			spdlog::info("'{}' {}:{} \"{}\"", diagnostic->GetToken()->GetFile(), diagnostic->GetToken()->GetLine(), diagnostic->GetToken()->GetColumn(), diagnostic->GetMessage());
			messageCount++;
			break;
		case Diagnostic::Warning:
			spdlog::warn("'{}' {}:{} \"{}\"", diagnostic->GetToken()->GetFile(), diagnostic->GetToken()->GetLine(), diagnostic->GetToken()->GetColumn(), diagnostic->GetMessage());
			warningCount++;
			break;
		case Diagnostic::Error:
			spdlog::error("'{}' {}:{} \"{}\"", diagnostic->GetToken()->GetFile(), diagnostic->GetToken()->GetLine(), diagnostic->GetToken()->GetColumn(), diagnostic->GetMessage());
			errCount++;
			break;
		}

	if (errCount)
	{
		spdlog::info("{} failed with {} errors, {} warnings, {} messages", step, errCount, warningCount, messageCount);

		return true;
	}
	else
	{
		spdlog::info("{} completed with {} errors, {} warnings, {} messages", step, errCount, warningCount, messageCount);

		return false;
	}
}
std::vector<shared_ptr<Token>> sTokens;

shared_ptr<TranslationUnitSyntax> sNode;

shared_ptr<BoundCompilationUnit> sBound;

void Lex()
{
	shared_ptr<Lexer> lexer = make_shared<Lexer>((char*)"sy/Main.sy");
	sTokens.clear();

	do
		sTokens.push_back(lexer->Lex());
	while (!sTokens.back()->Is(Token::EndOfFile));

	if (PrintDiagnosticBag(lexer->GetDiagnosticBag(), "Lexing"))
	{
		sTokens.clear();
		return;
	}

	spdlog::info("Lex Tokens:");
	for (auto tok : sTokens)
	{
		tok->Print(std::cout, "", tok->Is(Token::EndOfFile));
		putchar('\n');
	}
}

void Parse()
{
	if (sTokens.empty())
		return;

	shared_ptr<Parser> parser = make_shared<Parser>(sTokens);
	sNode = parser->Parse();
	putchar('\n');
	if (PrintDiagnosticBag(parser->GetDiagnosticBag(), "Parsing"))
	{
		sNode = nullptr;
		return;
	}

	spdlog::info("Parse Tree:");
	sNode->Print();
	putchar('\n');
}

void Bind()
{
	if (!sNode)
		return;

	shared_ptr<Binder> binder = make_shared<Binder>();
	sBound = binder->Bind(sNode);
	putchar('\n');
	if (PrintDiagnosticBag(binder->GetDiagnosticBag(), "Binding"))
	{
		sBound = false;
		return;
	}

	spdlog::info("Bound Tree:");
	sBound->Print();
	putchar('\n');

#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Reset);
#endif
}

void _Emit()
{
	if (!sBound)
		return;

	shared_ptr<Emitter> emmiter = make_shared<AsmEmitter>((char*)"sy/Main.S");
	emmiter->Emit(sBound);
	emmiter->Compile();
}

void LinkANDExec()
{
	if (!sBound)
		return;

	system("clang -m32 --optimize sy/Main.S -o sy/Main.exe");
	puts("Executing program...");
	int ec = system("sy\\Main");
	printf("\nProgram Exited with code %i (0x%x)", ec, ec);
}

int main()
{
	spdlog::set_pattern("[Symple]%^<%l>%$: %v");
	spdlog::set_level(level_enum::trace);
#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Yellow);
#endif

	Lex();
	Parse();
	Bind();
	_Emit();
	LinkANDExec();

	return !getc(stdin);
}