#include "SympleCode/Emit/AsmEmitter.h"

#include <sstream>
#include <spdlog/spdlog.h>

#include "SympleCode/Util/FileUtil.h"

#define _Emit(section, fmt, ...) ((void)fprintf(m##section##Stream, fmt "\n", ##__VA_ARGS__))

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

	char* AsmEmitter::RegDx(unsigned sz)
	{
		if (sz <= 1)
			return "%dl";
		else if (sz <= 2)
			return "%dx";
		else if (sz <= 4)
			return "%edx";

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


	std::string AsmEmitter::GetFunctionAssemblyName(shared_ptr<Symbol::FunctionSymbol> fn)
	{
		std::stringstream ss;
		switch (fn->GetCallingConvention())
		{
		case Symbol::FunctionSymbol::CDecl:
			ss << '_' << fn->GetName();
			break;
		case Symbol::FunctionSymbol::StdCall:
			ss << '_' << fn->GetName() << '@';
			unsigned numBytes = 0;
			for (auto param : fn->GetParameters())
				numBytes += param->GetType()->GetSize();
			ss << numBytes;
			break;
		}

		return ss.str();
	}


	AsmEmitter::AsmEmitter(char* file)
		: mFile(file)
	{
		if (mFile)
			mTextStream = Util::OpenFile(mFile, "wb+");
		else
			mTextStream = Util::OpenFile(mFile = "a.S", "wb+");
		mDataStream = Util::OpenTempFile();
		mBssStream = Util::OpenTempFile();

		_Emit(Text, ".text");
		_Emit(Data, ".data");
		_Emit(Bss, ".bss");
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
		if (!body) // External Function just ignore
			return;

		mFunction = func;
		std::string name = GetFunctionAssemblyName(func);

		_Emit(Text, ".global %s", name.c_str());
		std::stringstream fnSig;
		mFunction->PrintSignature(fnSig);
		_Emit(Text, "%s: # %s", name.c_str(), fnSig.str().c_str());
		_Emit(Text, "\t# Begin Stack Frame");
		_Emit(Text, "\tpush    %%ebp");
		_Emit(Text, "\tmov     %%esp, %%ebp");
		_Emit(Text, "\tsub     %s.StackSize, %%esp", name.c_str());
		mStackSize = 0;

		BeginScope();

		unsigned stackPos = 4;
		for (auto param : func->GetParameters())
		{
			stackPos += 4;
			_Emit(Text, "_%s$%i = -%i", param->GetName().data(), mScope->GetDepth(), stackPos);
			mScope->DeclareVariable(param);
		}

		mReturning = false;
		EmitStatement(body);

		EndScope();

		if (mReturning)
			_Emit(Text, "%s.Return:", name.c_str());
		_Emit(Text, "\t# End Stack Frame");
		_Emit(Text, "\tmov     %%ebp, %%esp");
		_Emit(Text, "\tpop     %%ebp");
		_Emit(Text, "\tret");

		_Emit(Data, "%s.StackSize:", name.c_str());
		_Emit(Data, "\t.long %i", mStackSize);
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
		case Binding::Node::VariableDeclaration:
			EmitVariableDeclaration(dynamic_pointer_cast<Binding::BoundVariableDeclaration>(stmt));
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
		_Emit(Text, "\tjmp     %s.Return", GetFunctionAssemblyName(mFunction).c_str());
		mReturning = true;
	}

	void AsmEmitter::EmitExpressionStatement(shared_ptr<Binding::BoundExpressionStatement> stmt)
	{ EmitExpression(stmt->GetExpression()); }

	void  AsmEmitter::EmitVariableDeclaration(shared_ptr<Binding::BoundVariableDeclaration> stmt)
	{
		std::string_view name = stmt->GetSymbol()->GetName();
		_Emit(Text, "_%s$%i = -%i", name.data(), mScope->GetDepth(), mStackSize);

		unsigned sz = stmt->GetSymbol()->GetType()->GetSize();
		mStackSize += sz;

		if (stmt->GetInitializer())
		{
			EmitExpression(stmt->GetInitializer());
			_Emit(Text, "\tmov     %s, _%s$%i(%%ebp)", RegAx(sz), name.data(), mScope->GetDepth());
		}

		mScope->DeclareVariable(stmt->GetSymbol());
	}


	void AsmEmitter::EmitConstant(shared_ptr<Binding::BoundConstant> val)
	{ _Emit(Text, "\tmov     $%i, %%eax", val->GetValue()); }

	shared_ptr<Symbol::TypeSymbol>  AsmEmitter::EmitExpression(shared_ptr<Binding::BoundExpression> expr)
	{
		if (expr->ConstantValue())
		{
			EmitConstant(expr->ConstantValue());
			return expr->GetType();
		}

		switch (expr->GetKind())
		{
		case Binding::Node::CallExpression:
			return EmitCallExpression(dynamic_pointer_cast<Binding::BoundCallExpression>(expr));
		case Binding::Node::UnaryExpression:
			return EmitUnaryExpression(dynamic_pointer_cast<Binding::BoundUnaryExpression> (expr));
		case Binding::Node::FunctionPointer:
			return EmitFunctionPointer(dynamic_pointer_cast<Binding::BoundFunctionPointer> (expr));
		case Binding::Node::BinaryExpression:
			return EmitBinaryExpression(dynamic_pointer_cast<Binding::BoundBinaryExpression> (expr));
		case Binding::Node::VariableExpression:
			return EmitVariableExpression(dynamic_pointer_cast<Binding::BoundVariableExpression> (expr));
		default:
			_Emit(Text, "\tmov     @@ERROR, %%eax");
			return Symbol::TypeSymbol::ErrorType;
		}
	}

	shared_ptr<Symbol::TypeSymbol> AsmEmitter::EmitCallExpression(shared_ptr<Binding::BoundCallExpression> expr)
	{
		for (unsigned i = expr->GetArguments().size(); i; i--)
		{
			EmitExpression(expr->GetArguments()[i - 1]);
			_Emit(Text, "\tpush    %%eax");
		}
		_Emit(Text, "\tcall    %s", GetFunctionAssemblyName(expr->GetFunction()).c_str());
		_Emit(Text, "\tadd     $%i, %%esp", expr->GetFunction()->GetParameters().size() * 4);

		return expr->GetFunction()->GetType();
	}

	shared_ptr<Symbol::TypeSymbol> AsmEmitter::EmitUnaryExpression(shared_ptr<Binding::BoundUnaryExpression> expr)
	{
		EmitExpression(expr->GetOperand());

		switch (expr->GetOperator()->GetKind())
		{
		case Binding::BoundUnaryOperator::Negative:
			_Emit(Text, "\tneg     %%eax");
			break;
		}

		return expr->GetType();
	}

	shared_ptr<Symbol::TypeSymbol> AsmEmitter::EmitFunctionPointer(shared_ptr<Binding::BoundFunctionPointer> expr)
	{
		_Emit(Text, "\tlea     %s, %%eax", GetFunctionAssemblyName(expr->GetSymbol()).c_str());

		return expr->GetType();
	}

	shared_ptr<Symbol::TypeSymbol> AsmEmitter::EmitBinaryExpression(shared_ptr<Binding::BoundBinaryExpression> expr)
	{
		EmitExpression(expr->GetRight());
		_Emit(Text, "\tpush    %%eax");

		if (expr->IsMutable()) // Assignment
		{
			EmitExpressionPointer(expr->GetLeft());
			_Emit(Text, "\tpop     %%edx");

			switch (expr->GetOperator()->GetKind())
			{
			case Binding::BoundBinaryOperator::Assign:
				_Emit(Text, "\tmov     %s, (%%eax)", RegDx(expr->GetLeft()->GetType()->GetSize()));
				break;
			}
		}
		else
		{
			EmitExpression(expr->GetLeft());
			_Emit(Text, "\tpop     %%edx");

			switch (expr->GetOperator()->GetKind())
			{
			case Binding::BoundBinaryOperator::Addition:
				_Emit(Text, "\tadd     %%edx, %%eax");
				break;
			case Binding::BoundBinaryOperator::Subtraction:
				_Emit(Text, "\tsub     %%edx, %%eax");
				break;
			case Binding::BoundBinaryOperator::Multiplication:
				_Emit(Text, "\timul    %%edx, %%eax");
				break;
			case Binding::BoundBinaryOperator::Division:
				_Emit(Text, "\tmov     %%edx, %%ecx");
				_Emit(Text, "\tcltd");
				_Emit(Text, "\tidiv    %%ecx");
				break;
			case Binding::BoundBinaryOperator::Modulo:
				_Emit(Text, "\tmov     %%edx, %%ecx");
				_Emit(Text, "\tcltd");
				_Emit(Text, "\tidiv    %%ecx");
				_Emit(Text, "\tmov     %%edx, %%eax");
				break;
			}
		}

		return expr->GetType();
	}

	shared_ptr<Symbol::TypeSymbol> AsmEmitter::EmitVariableExpression(shared_ptr<Binding::BoundVariableExpression> expr)
	{
		shared_ptr<Symbol::VariableSymbol> var = mScope->GetVariableSymbol(expr->GetSymbol()->GetName());
		if (var != expr->GetSymbol())
		{
			abort(); // Something bad, happening in code...
			return Symbol::TypeSymbol::ErrorType;
		}

		std::string_view name = var->GetName();
		unsigned depth = mScope->GetVariableDepth(var->GetName());
		unsigned sz = var->GetType()->GetSize();
		_Emit(Text, "\tmov     _%s$%i(%%ebp), %s", name.data(), depth, RegAx(sz));
		if (sz <= 2)
			_Emit(Text, "\tmovs%cl  %s, %%eax", Suf(sz), RegAx(sz));

		return expr->GetType();
	}


	shared_ptr<Symbol::TypeSymbol> AsmEmitter::EmitExpressionPointer(shared_ptr<Binding::BoundExpression> expr)
	{
		switch (expr->GetKind())
		{
		case Binding::Node::VariableExpression:
			return EmitVariableExpressionPointer(dynamic_pointer_cast<Binding::BoundVariableExpression>(expr));
		default:
			_Emit(Text, "\tmov     @@ERROR, %%eax");
			return Symbol::TypeSymbol::ErrorType;
		}
	}

	shared_ptr<Symbol::TypeSymbol> AsmEmitter::EmitVariableExpressionPointer(shared_ptr<Binding::BoundVariableExpression> expr)
	{
		shared_ptr<Symbol::VariableSymbol> var = mScope->GetVariableSymbol(expr->GetSymbol()->GetName());
		__SY_ASSERT(var == expr->GetSymbol(), "Internal Error");

		std::string_view name = var->GetName();
		unsigned depth = mScope->GetVariableDepth(var->GetName());
		_Emit(Text, "\tlea     _%s$%i(%%ebp), %%eax", name.data(), depth);

		return var->GetType();
	}


	void AsmEmitter::CloseStreams()
	{
		if (!mClosed)
		{
			rewind(mDataStream);
			Util::DumpFile(mDataStream, mTextStream);
			rewind(mBssStream);
			Util::DumpFile(mBssStream, mTextStream);

			// Print Output
			rewind(mTextStream);
			std::stringstream ss;
			ss << Util::ReadFile(mTextStream);
			Util::SetConsoleColor(Util::ConsoleColor::Yellow);
			spdlog::debug("Assembly Code:");
			Util::SetConsoleColor(Util::ConsoleColor::DarkYellow);
			spdlog::debug("{}", ss.str());
			Util::ResetConsoleColor();

			Util::CloseFile(mDataStream);
			Util::CloseFile(mBssStream);
			Util::CloseFile(mTextStream);

			mClosed = true;
		}
	}
}
