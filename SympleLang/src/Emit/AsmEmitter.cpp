#include "SympleCode/Emit/AsmEmitter.h"

#include <sstream>

#include "SympleCode/Util/FileUtil.h"

#define _Emit(section, fmt, ...) ((void)fprintf(m##section##Stream, fmt "\n", __VA_ARGS__))

namespace Symple::Emit
{
	AsmEmitter::AsmEmitter(char* file)
		: mFile(file)
	{
		if (mFile)
			mTextStream = Util::OpenFile(mFile, "wb+");
		else
			mTextStream = Util::OpenFile(mFile = "a.S", "wb+");
		//mTextStream = stdout;
		mDataStream = Util::OpenTempFile();
	}

	AsmEmitter::~AsmEmitter()
	{ CloseStreams(); }


	void AsmEmitter::Compile()
	{
		CloseStreams();
		std::stringstream cmd;
		std::string file = mFile;
		cmd << "clang -m32 --optimize -c " << mFile << " -o " << file.substr(0, file.find_last_of('.')) << ".o";
		system(cmd.str().c_str());
	}

	void AsmEmitter::Emit(shared_ptr<Binding::BoundCompilationUnit> unit)
	{
		for (auto func : unit->GetFunctions())
			EmitFunction(func.first, func.second);
	}

	void AsmEmitter::EmitFunction(shared_ptr<Symbol::FunctionSymbol> func, shared_ptr<Binding::BoundStatement> body)
	{
		mFunction = func;
		auto name = mFunction->GetName().data();

		_Emit(Text, ".global _%s", name);
		std::stringstream fnSig;
		mFunction->PrintSignature(fnSig);
		_Emit(Text, "_%s: # %s", name, fnSig.str().c_str());
		_Emit(Text, "\tpush    %%ebp");
		_Emit(Text, "\tmov     %%esp, %%ebp");
		_Emit(Text, "\t# Push Stack");

		mReturning = false;
		mReturn = !!dynamic_pointer_cast<Binding::BoundBlockStatement>(body);
		EmitStatement(body);

		if (mReturning)
			_Emit(Text, "_%s.Return:", name);
		_Emit(Text, "\t# Pop Stack");
		_Emit(Text, "\tmov     %%ebp, %%esp");
		_Emit(Text, "\tpop     %%ebp");
		_Emit(Text, "\tret");
	}


	void AsmEmitter::EmitStatement(shared_ptr<Binding::BoundStatement> stmt)
	{
		switch (stmt->GetKind())
		{
		case Binding::Node::ReturnStatement:
			EmitReturnStatement(dynamic_pointer_cast<Binding::BoundReturnStatement>(stmt));
			break;
		}
	}

	void AsmEmitter::EmitReturnStatement(shared_ptr<Binding::BoundReturnStatement> stmt)
	{
		EmitExpression(stmt->GetValue());
		if (mReturning = mReturn)
			_Emit(Text, "\tjmp     _%s.Return", mFunction->GetName().data());
	}


	void AsmEmitter::EmitExpression(shared_ptr<Binding::BoundExpression> expr)
	{
		if (expr->ConstantValue())
			_Emit(Text, "\tmov     $%i, %%eax", expr->ConstantValue()->GetValue());

		switch (expr->GetKind())
		{
		case Binding::Node::CallExpression:
			EmitCallExpression(dynamic_pointer_cast<Binding::BoundCallExpression>(expr));
			break;
		}
	}

	void AsmEmitter::EmitCallExpression(shared_ptr<Binding::BoundCallExpression> expr)
	{
		for (auto arg : expr->GetArguments())
		{
			EmitExpression(arg);
			_Emit(Text, "\tpush    %%eax");
		}
		_Emit(Text, "\tcall    _%s", expr->GetFunction()->GetName().data());
		_Emit(Text, "\tadd     $%i, %%esp", expr->GetArguments().size() * 4);
	}


	void AsmEmitter::CloseStreams()
	{
		if (!mClosed)
		{
			rewind(mDataStream);

			char c;
			while ((c = fgetc(mDataStream)) != EOF)
				fputc(c, mTextStream);

			Util::CloseFile(mDataStream);
			rewind(mTextStream);
			puts(Util::ReadFile(mTextStream).c_str());

			Util::CloseFile(mTextStream);

			mClosed = true;
		}
	}
}