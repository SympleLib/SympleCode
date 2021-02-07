#include "SympleCode/Emit/AsmEmitter.h"

#include <sstream>

#include "SympleCode/Util/FileUtil.h"

#define _Emit(section, fmt, ...) ((void)fprintf(m##section##Stream, fmt "\n", __VA_ARGS__))

namespace Symple::Emit
{
	void AsmEmitter::BeginScope()
	{ mScope = make_shared<Scope>(mScope); }

	void AsmEmitter::EndScope()
	{ mScope = mScope->GetBase(); }

	char* AsmEmitter::RegAx(unsigned sz)
	{
		if (sz <= 1)
			return "%al";
		else if (sz <= 2)
			return "%ax";
		else if (sz <= 4)
			return "%eax";

		return nullptr;
	}

	char AsmEmitter::Suf(unsigned sz)
	{
		if (sz <= 1)
			return 'b';
		else if (sz <= 2)
			return 'w';
		else if (sz <= 4)
			return 'l';

		return 0xCC;
	}

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
		cmd << "clang -m32 --optimize -c " << mFile << " -o " << file.substr(0, file.find_last_of('.')) << ".obj";
		system(cmd.str().c_str());
	}

	void AsmEmitter::Emit(shared_ptr<Binding::BoundCompilationUnit> unit)
	{
		mCompilationUnit = unit;

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

		BeginScope();

		unsigned stackPos = 4;
		for (auto param : func->GetParameters())
		{
			stackPos += 4;
			_Emit(Text, "_%s$0 = %i", param->GetName().data(), stackPos);
			mScope->DeclareVariable(param);
		}

		mReturning = false;
		EmitStatement(body);

		EndScope();

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
		case Binding::Node::BlockStatement:
			EmitBlockStatement(dynamic_pointer_cast<Binding::BoundBlockStatement>(stmt));
			break;
		case Binding::Node::ReturnStatement:
			EmitReturnStatement(dynamic_pointer_cast<Binding::BoundReturnStatement>(stmt));
			break;
		case Binding::Node::ExpressionStatement:
			EmitExpressionStatement(dynamic_pointer_cast<Binding::BoundExpressionStatement>(stmt));
			break;
		}
	}

	void AsmEmitter::EmitBlockStatement(shared_ptr<Binding::BoundBlockStatement> stmt)
	{
		BeginScope();

		for (auto ln : stmt->GetStatements())
			EmitStatement(ln);

		EndScope();
	}

	void AsmEmitter::EmitReturnStatement(shared_ptr<Binding::BoundReturnStatement> stmt)
	{
		EmitExpression(stmt->GetValue());
		_Emit(Text, "\tjmp     _%s.Return", mFunction->GetName().data());
		mReturning = true;
	}

	void AsmEmitter::EmitExpressionStatement(shared_ptr<Binding::BoundExpressionStatement> stmt)
	{ EmitExpression(stmt->GetExpression()); }


	void AsmEmitter::EmitConstant(shared_ptr<Binding::BoundConstant> val)
	{ _Emit(Text, "\tmov     $%i, %%eax", val->GetValue()); }

	void AsmEmitter::EmitExpression(shared_ptr<Binding::BoundExpression> expr)
	{
		if (expr->ConstantValue())
			return EmitConstant(expr->ConstantValue());

		switch (expr->GetKind())
		{
		case Binding::Node::CallExpression:
			return EmitCallExpression(dynamic_pointer_cast<Binding::BoundCallExpression>(expr));
		case Binding::Node::VariableExpression:
			return EmitVariableExpression(dynamic_pointer_cast<Binding::BoundVariableExpression> (expr));
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
		_Emit(Text, "\tadd     $%i, %%esp", expr->GetFunction()->GetParameters().size() * 4);
	}

	void AsmEmitter::EmitVariableExpression(shared_ptr<Binding::BoundVariableExpression> expr)
	{
		shared_ptr<Symbol::VariableSymbol> var = mScope->GetVariableSymbol(expr->GetSymbol()->GetName());
		if (var != expr->GetSymbol())
		{
			abort(); // Something bad, happening in code...
			return;
		}

		std::string_view name = var->GetName();
		unsigned depth = mScope->GetVariableDepth(var->GetName());
		unsigned sz = var->GetType()->GetSize();
		_Emit(Text, "\tmov     _%s$%i(%%ebp), %s", name.data(), depth, RegAx(sz));
		if (sz <= 2)
			_Emit(Text, "\tmovs%cl  %s, %%eax", Suf(sz), RegAx(sz));
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