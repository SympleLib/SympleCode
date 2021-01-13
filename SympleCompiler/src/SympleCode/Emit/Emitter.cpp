#include "SympleCode/Emit/Emitter.h"

#include <Windows.h>

#define Emit(fmt, ...) fprintf(mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__)

#define GetReg RegisterManager::GetRegister

namespace Symple
{
	unsigned int Emitter::sInits = 0;

	Emitter::Emitter(const char* path)
		: mPath(path), mFile(), mLiteralFile(), mData(), mReturn(), mStack(),
			mReturning(), mRegisterManager(new RegisterManager(this))
	{
		while (OpenFile());
		while (OpenLiteralFile());
	}

	Emitter::~Emitter()
	{
		rewind(mLiteralFile);

		char c;
		while ((c = getc(mLiteralFile)) != EOF)
			putc(c, mFile);

		fclose(mLiteralFile);
		fclose(mFile);
	}

	void Emitter::EmitCompilationUnit(const CompilationUnitNode* unit)
	{
		for (const MemberNode* member : unit->GetMembers())
			EmitMember(member);
	}

	void Emitter::EmitStaticInitialization()
	{
		Emit("\t.globl   _main");
		Emit("_main:");

		for (unsigned int i = 0; i < sInits; i++)
			Emit("\tcall%c   ..%i.", Suf(), i);

		Emit("\txor%c    %s, %s", Suf(), GetReg(regax), GetReg(regax));
		Emit("\tcall%c   main", Suf());

		Emit("\tret");
	}


	char Emitter::Suf(int sz)
	{
		if (sz <= 1)
			return 'b';
		if (sz <= 2)
			return 'w';
		if (sz <= 4)
			return 'l';
#if SY_64
		if (sz <= 8)
			return 'q';
#endif

		return 0;
	}

	const char* Emitter::Word(int sz)
	{
		if (sz <= 1)
			return "byte";
		if (sz <= 2)
			return "value";
		if (sz <= 4)
			return "long";
#if SY_64
		if (sz <= 8)
			return "quad";
#endif

		return nullptr;
	}

	void Emitter::Push(Register reg)
	{
		Emit("\tpush%c   %s", Suf(), GetReg(reg));
	}

	void Emitter::Pop(Register reg)
	{
		Emit("\tpop%c    %s", Suf(), GetReg(reg));
	}

	void Emitter::EmitMember(const MemberNode* member)
	{
		Emit("\t# Member of Kind: %s", Node::KindString(member->GetKind()));

		if (member->Is<GlobalStatementNode>())
			EmitGlobalStatement(member->Cast<GlobalStatementNode>());
		if (member->Is<FunctionDeclarationNode>())
			EmitFunctionDeclaration(member->Cast<FunctionDeclarationNode>());
		if (member->Is<GlobalVariableDeclarationNode>())
			EmitGlobalVariableDeclaration(member->Cast<GlobalVariableDeclarationNode>());
	}

	void Emitter::EmitGlobalStatement(const GlobalStatementNode* member)
	{
		EmitStatement(member->GetStatement());
	}

	void Emitter::EmitFunctionDeclaration(const FunctionDeclarationNode* member)
	{
		if (member->GetKind() != Node::Kind::FunctionDeclaration)
			return;

		Debug::BeginScope();

		int argOff = 4;
		for (const FunctionArgumentNode* arg : member->GetArguments()->GetArguments())
		{
			argOff += 4;
			Debug::VariableDeclaration(arg);
			Emit("_%s@ = %i", std::string(arg->GetName()->GetLex()).c_str(), argOff);
		}

		const char* name = member->GetAsmName().c_str();

		mStack = 0;
		if (member->IsMain())
		{
			Emit("\t.globl   main");
			Emit("main:");
		}
		else
		{
			if (member->GetModifiers()->IsPrivate())
				Emit("\t.local   %s .type %s, @function", name, name);
			else
				Emit("\t.globl   %s", name);
			Emit("%s:", name);
		}
		Emit("\tpush%c   %s", Suf(), GetReg(regbp));
		Emit("\tmov%c    %s, %s", Suf(), GetReg(regsp), GetReg(regbp));

		mReturning = false;
		for (const StatementNode* statement : member->GetBody()->GetStatements())
			if (statement->IsReturn() && statement != member->GetBody()->GetStatements().back())
			{
				mReturning = true;
				break;
			}

		if (mReturning)
			mReturn = mData++;

		EmitBlockStatement(member->GetBody());
		Debug::EndScope();

		if (mReturning)
			Emit("..%i:", mReturn);
		Emit("\tmov%c    %s, %s", Suf(), GetReg(regbp), GetReg(regsp));
		Emit("\tpop%c    %s", Suf(), GetReg(regbp));
		Emit("\tret");
	}

	void Emitter::EmitGlobalVariableDeclaration(const GlobalVariableDeclarationNode* member)
	{
		Debug::VariableDeclaration(member);

		if (member->GetKind() == Node::Kind::SharedVariable)
			return;

		std::string nstr(member->GetName()->GetLex());
		const char* name = nstr.c_str();

		if (member->GetType()->GetModifiers()->IsMutable())
			Emit("\t.bss");
		if (member->GetModifiers()->IsPrivate())
			Emit("\t.local   _%s@", name);
		else
			Emit("\t.globl   _%s@", name);
		Emit("_%s@:", name);

		if (member->GetInitializer())
		{
			if (member->GetInitializer()->CanEvaluate())
			{
				if (member->GetInitializer()->Is<StructInitializerExpressionNode>())
				{
					for (const ExpressionNode* item : member->GetInitializer()->Cast<StructInitializerExpressionNode>()->GetExpressions())
					{
						if (item->Is<StringLiteralExpressionNode>())
						{
							EmitLiteral("..%i:", mData);
							EmitLiteral("\t.string  \"%s\"", std::string(item->Cast<StringLiteralExpressionNode>()->GetLiteral()->GetLex()).c_str());

							Emit("\t.%s ..%i", Word(), mData++);
						}
						else
						{
							unsigned int sz = item->GetType()->GetSize();

							Emit("\t.%s %i", Word(sz), item->Evaluate());
						}
					}
				}
				else
				{
					unsigned int sz = member->GetType()->GetSize();
					
					Emit("\t.%s %i", Word(sz), member->GetInitializer()->Evaluate());
				}

				Emit("\t.text");
			}
			else if (member->GetInitializer()->Is<StringLiteralExpressionNode>())
			{
				EmitLiteral("..%i:", mData);
				EmitLiteral("\t.string  \"%s\"", std::string(member->GetInitializer()->Cast<StringLiteralExpressionNode>()->GetLiteral()->GetLex()).c_str());

				Emit("\t.%s ..%i", Word(), mData++);
			}
			else
			{
				Emit("\t.zero %i", member->GetType()->GetSize());

				Emit("\t.text");

				Emit("\t.globl   ..%i.", sInits);
				Emit("..%i.:", sInits++);
				if (member->GetInitializer()->Is<StructInitializerExpressionNode>())
				{
					Register ptr = mRegisterManager->Alloc();
					Emit("\tmov%c    $_%s@, %s", Suf(), name, GetReg(ptr));
					EmitStructInitializerExpression(member->GetInitializer()->Cast<StructInitializerExpressionNode>(), ptr);
					mRegisterManager->Free(ptr);
				}
				else
				{
					unsigned int sz = member->GetType()->GetSize();

					Register init = EmitExpression(member->GetInitializer());
					Emit("\tmov%c    %s, _%s@", Suf(sz), GetReg(init, sz), name);
					mRegisterManager->Free(init);
				}
				Emit("\tret");
			}
		}

		if (member->GetNext())
			EmitGlobalVariableDeclaration(member->GetNext());
	}


	void Emitter::EmitStatement(const StatementNode* statement)
	{
		Emit("\t# Statement of Kind: %s", Node::KindString(statement->GetKind()));

		if (statement->Is<IfStatementNode>())
			EmitIfStatement(statement->Cast<IfStatementNode>());
		if (statement->Is<AsmStatementNode>())
			EmitAsmStatement(statement->Cast<AsmStatementNode>());
		if (statement->Is<BlockStatementNode>())
			EmitBlockStatement(statement->Cast<BlockStatementNode>());
		if (statement->Is<BreakStatementNode>())
			EmitBreakStatement(statement->Cast<BreakStatementNode>());
		if (statement->Is<WhileStatementNode>())
			EmitWhileStatement(statement->Cast<WhileStatementNode>());
		if (statement->Is<ReturnStatementNode>())
			EmitReturnStatement(statement->Cast<ReturnStatementNode>());
		if (statement->Is<ForLoopStatementNode>())
			EmitForLoopStatement(statement->Cast<ForLoopStatementNode>());
		if (statement->Is<ExpressionStatementNode>())
			EmitExpressionStatement(statement->Cast<ExpressionStatementNode>());
		if (statement->Is<VariableDeclarationNode>())
			EmitVariableDeclaration(statement->Cast<VariableDeclarationNode>());
	}

	void Emitter::EmitIfStatement(const IfStatementNode* statement)
	{
		if (statement->GetElse())
		{
			unsigned int elze = mData++, end = mData++;

			Register cond = EmitExpression(statement->GetCondition());

			Emit("\ttest%c   %s, %s", Suf(), GetReg(cond), GetReg(cond));
			Emit("\tje      ..%i", elze);
			mRegisterManager->Free(cond);

			EmitBlockStatement(statement->GetThen());

			Emit("\tjmp     ..%i", end);
			Emit("..%i:", elze);

			EmitBlockStatement(statement->GetElse());

			Emit("..%i:", end);
		}
		else
		{
			unsigned int end = mData++;

			Register cond = EmitExpression(statement->GetCondition());

			Emit("\ttest%c   %s, %s", Suf(), GetReg(cond), GetReg(cond));
			Emit("\tje      ..%i", end);
			mRegisterManager->Free(cond);

			EmitBlockStatement(statement->GetThen());

			Emit("..%i:", end);
		}
	}

	void Emitter::EmitAsmStatement(const AsmStatementNode* statement)
	{
		Emit("%s", std::string(statement->GetInstructions()->GetLex()).c_str());
	}

	void Emitter::EmitBlockStatement(const BlockStatementNode* block, bool funcdecl)
	{
		if (block->GetStackUsage())
		{
			Emit("\tsub%c    $%i, %s", Suf(), block->GetStackUsage(), GetReg(regsp));
			mStack += block->GetStackUsage();
		}

		Debug::BeginScope();

		for (const StatementNode* statement : block->GetStatements())
			EmitStatement(statement);

		Debug::EndScope();

		mRegisterManager->FreeAll();
	}

	void Emitter::EmitBreakStatement(const BreakStatementNode* statement)
	{
		Emit("\tjmp     ..%i", mBreak);
	}

	void Emitter::EmitWhileStatement(const WhileStatementNode* statement)
	{
		unsigned int loop = mData++;
		mBreak = mData++;

		Emit("..%i:", loop);

		Register cond = EmitExpression(statement->GetCondition());
		Emit("\ttest%c   %s, %s", Suf(), GetReg(cond), GetReg(cond));
		Emit("\tjne     ..%i", mBreak);
		mRegisterManager->Free(cond);

		EmitBlockStatement(statement->GetBody());

		Emit("\tjmp     ..%i", loop);
		Emit("..%i:", mBreak);

	}

	void Emitter::EmitReturnStatement(const ReturnStatementNode* statement)
	{
		Register exprReg = EmitExpression(statement->GetExpression());
		if (exprReg != regax)
		{
			Register reg = mRegisterManager->Alloc(regax);
			Emit("\tmov%c    %s, %s", Suf(), GetReg(exprReg), GetReg(reg));

			mRegisterManager->Free(reg);
		}

		mRegisterManager->Free(exprReg);

		if (mReturning)
			Emit("\tjmp     ..%i", mReturn);
	}

	void Emitter::EmitForLoopStatement(const ForLoopStatementNode* statement)
	{
		unsigned int loop = mData++;
		mBreak = mData++;

		EmitStatement(statement->GetInitializer());

		Emit("..%i:", loop);

		Debug::BeginScope();

		Register cond = EmitExpression(statement->GetCondition());
		Emit("\ttest%c   %s, %s", Suf(), GetReg(cond), GetReg(cond));
		Emit("\tjne     ..%i", mBreak);
		mRegisterManager->Free(cond);

		EmitBlockStatement(statement->GetBody());
		EmitStatement(statement->GetStep());
		Debug::EndScope();

		Emit("\tjmp     ..%i", loop);
		Emit("..%i:", mBreak);
	}

	void Emitter::EmitExpressionStatement(const ExpressionStatementNode* statement)
	{
		Register exprReg = EmitExpression(statement->GetExpression());
		if (exprReg != regax)
		{
			Register reg = mRegisterManager->Alloc(regax);
			Emit("\tmov%c    %s, %s", Suf(), GetReg(exprReg), GetReg(reg));

			mRegisterManager->Free(reg);
		}

		mRegisterManager->Free(exprReg);
	}

	void Emitter::EmitVariableDeclaration(const VariableDeclarationNode* statement)
	{
		Debug::VariableDeclaration(statement);

		std::string nameStr(statement->GetName()->GetLex());
		const char* name = nameStr.c_str();

		Emit("_%s@ = -%i", name, mStack);
		unsigned int sz = statement->GetType()->GetSize();
		mStack += sz;
		if (statement->GetInitializer())
		{
			if (statement->GetInitializer()->Is<StructInitializerExpressionNode>())
			{
				Register ptr = mRegisterManager->Alloc();
				Emit("\tlea%c    _%s@(%s), %s", Suf(), name, GetReg(regbp), GetReg(ptr));
				EmitStructInitializerExpression(statement->GetInitializer()->Cast<StructInitializerExpressionNode>(), ptr);
				mRegisterManager->Free(ptr);
			}
			else
			{
				Register init = EmitExpression(statement->GetInitializer());
				Emit("\tmov%c    %s, _%s@(%s)", Suf(sz), GetReg(init, sz), name, GetReg(regbp));
				mRegisterManager->Free(init);
			}
		}

		if (statement->GetNext())
			EmitVariableDeclaration(statement->GetNext());
	}


	Register Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->CanEvaluate())
		{
			Emit("\t# Expression of Kind: %s = %i", Node::KindString(expression->GetKind()), expression->Evaluate());

			Register reg = mRegisterManager->Alloc();

			if (expression->Evaluate())
				Emit("\tmov%c    $%d, %s", Suf(), expression->Evaluate(), GetReg(reg));
			else
				Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));
			return reg;
		}

		Emit("\t# Expression of Kind: %s", Node::KindString(expression->GetKind()));

		if (expression->Is<CastExpressionNode>())
			return EmitCastExpression(expression->Cast<CastExpressionNode>());
		if (expression->Is<ListExpressionNode>())
			return EmitListExpression(expression->Cast<ListExpressionNode>());
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>());
		if (expression->Is<TernaryExpressionNode>())
			return EmitTernaryExpression(expression->Cast<TernaryExpressionNode>());
		if (expression->Is<OperatorExpressionNode>())
			return EmitOperatorExpression(expression->Cast<OperatorExpressionNode>());
		if (expression->Is<ModifiableExpressionNode>())
			return EmitModifiableExpression(expression->Cast<ModifiableExpressionNode>());
		if (expression->Is<FunctionCallExpressionNode>())
			return EmitFunctionCallExpression(expression->Cast<FunctionCallExpressionNode>());
		if (expression->Is<ParenthesizedExpressionNode>())
			return EmitParenthesizedExpression(expression->Cast<ParenthesizedExpressionNode>());
		
		return nullreg;
	}

	Register Emitter::EmitCastExpression(const CastExpressionNode* expression)
	{
		return EmitExpression(expression->GetExpression());
	}

	Register Emitter::EmitListExpression(const ListExpressionNode* expression)
	{
		Register reg = mRegisterManager->CAlloc();
		if (expression->GetExpressions().empty())
			return reg;

		unsigned int sz = expression->GetExpressions().size() * expression->GetExpressionType()->GetSize();
		Emit("\tsub%c   $%i, %s", Suf(), sz, GetReg(regsp));

		mStack += sz;
		for (const ExpressionNode* item : expression->GetExpressions())
		{
			Register itemReg = EmitExpression(item);
			Emit("\tmov%c    %s, -%i(%s)", Suf(), GetReg(itemReg), mStack, GetReg(regbp));
			mRegisterManager->Free(itemReg);

			mStack -= item->GetType()->GetSize();
		}
		mStack += sz;

		Emit("\tlea%c    -%i(%s), %s", Suf(), mStack, GetReg(regbp), GetReg(reg));
		return reg;
	}

	Register Emitter::EmitTernaryExpression(const TernaryExpressionNode* expression)
	{
		unsigned int elze = mData++, end = mData++;

		Register reg = EmitExpression(expression->GetCondition());
		Emit("\ttest%c   %s, %s", Suf(), GetReg(reg), GetReg(reg));
		Emit("\tje      ..%i", elze);
		mRegisterManager->Free(reg);

		Register then = EmitExpression(expression->GetThen());
		if (then != reg)
		{
			reg = mRegisterManager->Alloc();
			Emit("\tmov%c    %s, %s", Suf(), GetReg(then), GetReg(reg));
			mRegisterManager->Free(reg);
		}
		mRegisterManager->Free(then);

		Emit("\tjmp     ..%i", end);
		Emit("..%i:", elze);

		Register elzeReg = EmitExpression(expression->GetElse());
		if (elzeReg == reg)
		{
			mRegisterManager->Free(elzeReg);
			reg = mRegisterManager->Alloc();
		}
		else
		{
			reg = mRegisterManager->Alloc();
			Emit("\tmov%c    %s, %s", Suf(), GetReg(elzeReg), GetReg(reg));
			mRegisterManager->Free(elzeReg);
		}

		Emit("..%i:", end);

		return reg;
	}

	Register Emitter::EmitFunctionCallExpression(const FunctionCallExpressionNode* expression)
	{
		for (int i = 0; i < NumRegisters; i++)
			if (!mRegisterManager->GetFree()[i])
			{
				if (i == regax)
				{
					Register reg = mRegisterManager->Alloc();
					mRegisterManager->Free(i);
					Emit("\tmov%c    %s, %s", Suf(), GetReg(regax), GetReg(reg));
					Push(reg);
				}
				else
					Push(i);
			}

		for (unsigned int i = expression->GetArguments()->GetArguments().size(); i; i--)
		{
			Register argReg = EmitExpression(expression->GetArguments()->GetArguments()[i - 1]);
			Push(argReg);
			mRegisterManager->Free(argReg);
		}

		const FunctionDeclarationNode* function = Debug::GetFunction(expression->GetName()->GetLex(), expression->GetArguments());

		Emit("\tcall%c   %s", Suf(), function->GetAsmName().c_str());
		if (expression->GetArguments()->GetArguments().size())
			Emit("\tadd%c    $%i, %s", Suf(), expression->GetArguments()->GetArguments().size() * 4, GetReg(regsp));

		for (int i = 0; i < NumRegisters; i++)
			if (i != regax && !mRegisterManager->GetFree()[i])
				Pop(i);

		return mRegisterManager->Alloc(regax);
	}

	Register Emitter::EmitParenthesizedExpression(const ParenthesizedExpressionNode* expression)
	{
		return EmitExpression(expression->GetExpression());
	}

	Register Emitter::EmitVariableAddressExpression(const VariableAddressExpressionNode* expression)
	{
		return EmitModifiableExpression(expression->GetVariable(), true);
	}

	void Emitter::EmitStructInitializerExpression(const StructInitializerExpressionNode* expression, Register to)
	{
		int off = 0;
		for (const ExpressionNode* item : expression->GetExpressions())
		{
			Register reg = EmitExpression(item);
			Emit("\tmov%c    %s, %i(%s)", Suf(), GetReg(reg), off, GetReg(to));
			mRegisterManager->Free(reg);

			off += item->GetType()->GetSize();
		}
	}

	void Emitter::EmitStructInitializerExpression(const StructInitializerExpressionNode* expression, const ModifiableExpressionNode* exprTo)
	{
		Register to = EmitModifiableExpression(exprTo, true);

		EmitStructInitializerExpression(expression, to);

		mRegisterManager->Free(to);
	}


	Register Emitter::EmitModifiableExpression(const ModifiableExpressionNode* expression, bool retptr)
	{
		if (expression->Is<FieldExpressionNode>())
			return EmitFieldExpression(expression->Cast<FieldExpressionNode>(), retptr);
		if (expression->Is<VariableExpressionNode>())
			return EmitVariableExpression(expression->Cast<VariableExpressionNode>(), retptr);
		if (expression->Is<AssignmentExpressionNode>())
			return EmitAssignmentExpression(expression->Cast<AssignmentExpressionNode>(), retptr);
		if (expression->Is<PointerIndexExpressionNode>())
			return EmitPointerIndexExpression(expression->Cast<PointerIndexExpressionNode>(), retptr);
		if (expression->Is<DereferencePointerExpressionNode>())
			return EmitDereferencePointerExpression(expression->Cast<DereferencePointerExpressionNode>(), retptr);

		return nullreg;
	}

	Register Emitter::EmitFieldExpression(const FieldExpressionNode* expression, bool retptr)
	{
		Register callee = EmitModifiableExpression(expression->GetCallee(), true);
		unsigned int off = expression->GetOffset();

		if (retptr)
		{
			Emit("\tadd%c    $%i, %s", Suf(), off, GetReg(callee));
			return callee;
		}

		Register val = mRegisterManager->CAlloc();
		unsigned int sz = expression->GetType()->GetSize();
		Emit("\tmov%c    %i(%s), %s", Suf(sz), off, GetReg(callee), GetReg(val, sz));
		mRegisterManager->Free(callee);
		return val;
	}

	Register Emitter::EmitVariableExpression(const VariableExpressionNode* expression, bool retptr)
	{
		const VariableDeclaration* var = Debug::GetVariable(expression->GetName()->GetLex());
		Register reg = mRegisterManager->Alloc();

		if (!var->Is<GlobalVariableDeclarationNode>())
		{
			if (retptr)
				Emit("\tlea%c    _%s@(%s), %s", Suf(), std::string(expression->GetName()->GetLex()).c_str(), GetReg(regbp), GetReg(reg));
			else
			{
				unsigned int sz = var->GetType()->GetSize();

				if (sz != 4)
					Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));
				Emit("\tmov%c    _%s@(%s), %s", Suf(sz), std::string(expression->GetName()->GetLex()).c_str(), GetReg(regbp), GetReg(reg, sz));
			}

			return reg;
		}

		if (retptr)
			Emit("\tmov%c    $_%s@, %s", Suf(), std::string(expression->GetName()->GetLex()).c_str(), GetReg(reg));
		else
		{
			if (var->GetType()->GetType()->Is<StructDeclarationNode>() && !var->GetType()->HasContinue(Token::Kind::Asterisk))
				// Just move a pointer since you cant cast a struct to any other type
				Emit("\tmov%c    $_%s@, %s", Suf(), std::string(expression->GetName()->GetLex()).c_str(), GetReg(reg));
			else
			{
				unsigned int sz = var->GetType()->GetSize();

				if (sz != 4)
					Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));
				Emit("\tmov%c    _%s@, %s", Suf(sz), std::string(expression->GetName()->GetLex()).c_str(), GetReg(reg, sz));
			}
		}

		return reg;
	}

	Register Emitter::EmitAssignmentExpression(const AssignmentExpressionNode* expression, bool retptr)
	{

		unsigned int sz = expression->GetLeft()->GetType()->GetSize();
		Register left = EmitModifiableExpression(expression->GetLeft(), true);
		Register right = EmitExpression(expression->GetRight());

		if (expression->GetRight()->Is<StructInitializerExpressionNode>())
		{
			EmitStructInitializerExpression(expression->GetRight()->Cast<StructInitializerExpressionNode>(), expression->GetLeft());
			goto Ret;
		}

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Equal:
			Emit("\tmov%c    %s, (%s)", Suf(sz), GetReg(right, sz), GetReg(left));
			goto Ret;
		case Token::Kind::PlusEqual:
			Emit("\tadd%c    %s, (%s)", Suf(sz), GetReg(right, sz), GetReg(left));
			goto Ret;
		case Token::Kind::MinusEqual:
			Emit("\tsub%c    %s, (%s)", Suf(sz), GetReg(right, sz), GetReg(left));
			goto Ret;
		case Token::Kind::AsteriskEqual:
			Emit("\tmul%c    %s, (%s)", Suf(sz), GetReg(right, sz), GetReg(left));
			goto Ret;
		}

		return nullreg;

	Ret:
		if (retptr)
		{
			mRegisterManager->Free(right);
			return left;
		}

		Register reg = mRegisterManager->Alloc();
		Emit("\tmov%c    (%s), %s", Suf(sz), GetReg(left), GetReg(reg, sz));
		mRegisterManager->Free(left);
		mRegisterManager->Free(right);
		return reg;
	}

	Register Emitter::EmitPointerIndexExpression(const PointerIndexExpressionNode* expression, bool retptr)
	{
		Register reg = EmitExpression(expression->GetAddress());
		if (expression->GetType()->GetSize())
			if (expression->GetIndex()->CanEvaluate())
			{
				if (expression->GetIndex()->Evaluate())
					Emit("\tadd%c    $%i, %s", Suf(), expression->GetIndex()->Evaluate() * expression->GetType()->GetSize(), GetReg(reg));
			}
			else
			{
				Register off = EmitExpression(expression->GetIndex());
				if (expression->GetType()->GetSize() > 1)
					Emit("\timul%c   $%i, %s", Suf(), expression->GetType()->GetSize(), GetReg(off));
				Emit("\tadd%c    %s, %s", Suf(), GetReg(off), GetReg(reg));
				mRegisterManager->Free(off);
			}

		if (retptr)
			return reg;

		Register val = mRegisterManager->CAlloc();
		unsigned int sz = expression->GetType()->GetSize();
		Emit("\tmov%c    (%s), %s", Suf(sz), GetReg(reg), GetReg(val, sz));
		mRegisterManager->Free(reg);
		return val;
	}

	Register Emitter::EmitDereferencePointerExpression(const DereferencePointerExpressionNode* expression, bool retptr)
	{
		Register reg = EmitExpression(expression->GetAddress());

		if (retptr)
			return reg;

		Register val = mRegisterManager->CAlloc();
		unsigned int sz = expression->GetType()->GetSize();
		Emit("\tmov%c    (%s), %s", Suf(sz), GetReg(reg), GetReg(val, sz));
		return val;
	}


	Register Emitter::EmitOperatorExpression(const OperatorExpressionNode* expression)
	{
		if (expression->Is<UnaryExpressionNode>())
			return EmitUnaryExpression(expression->Cast<UnaryExpressionNode>());
		if (expression->Is<BinaryExpressionNode>())
			return EmitBinaryExpression(expression->Cast<BinaryExpressionNode>());

		return nullreg;
	}

	Register Emitter::EmitUnaryExpression(const UnaryExpressionNode* expression)
	{
		Register reg = EmitExpression(expression->GetValue());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Exclamation:
		{
			Register out = mRegisterManager->CAlloc();

			Emit("\tcmp%c    $0, %s", Suf(), GetReg(reg));
			mRegisterManager->Free(reg);
			Emit("\tsete    %s", GetReg(out, 1));
			return out;
		}
		case Token::Kind::Minus:
			Emit("\tneg%c    %s", Suf(), GetReg(reg));
			return reg;
		}

		return nullreg;
	}

	Register Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression)
	{
		Register right = EmitExpression(expression->GetRight());
		Register left = EmitExpression(expression->GetLeft());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Plus:
			Emit("\tadd%c    %s, %s", Suf(), GetReg(right), GetReg(left));
			goto Ret;
		case Token::Kind::Minus:
			Emit("\tsub%c    %s, %s", Suf(), GetReg(right), GetReg(left));
			goto Ret;
		case Token::Kind::Asterisk:
			Emit("\timul%c   %s, %s", Suf(), GetReg(right), GetReg(left));
			goto Ret;

		case Token::Kind::EqualEqual:
			Emit("\tcmp%c    %s, %s", Suf(), GetReg(left), GetReg(right));
			Emit("\tsete    %s", GetReg(left, 1));
			Emit("\tmovz%c%c  %s, %s", Suf(1), Suf(), GetReg(left, 1), GetReg(left));
			goto Ret;
		case Token::Kind::ExclamationEqual:
			Emit("\tcmp%c    %s, %s", Suf(), GetReg(left), GetReg(right));
			Emit("\tsetne   %s", GetReg(left, 1));
			Emit("\tmovz%c%c  %s, %s", Suf(1), Suf(), GetReg(left, 1), GetReg(left));
			goto Ret;
		case Token::Kind::LeftArrow:
			Emit("\tcmp%c    %s, %s", Suf(), GetReg(left), GetReg(right));
			Emit("\tsetl    %s", GetReg(left, 1));
			Emit("\tmovz%c%c  %s, %s", Suf(1), Suf(), GetReg(left, 1), GetReg(left));
			goto Ret;
		case Token::Kind::LeftArrowEqual:
			Emit("\tcmp%c    %s, %s", Suf(), GetReg(left), GetReg(right));
			Emit("\tsetle   %s", GetReg(left, 1));
			Emit("\tmovz%c%c  %s, %s", Suf(1), Suf(), GetReg(left, 1), GetReg(left));
			goto Ret;
		case Token::Kind::RightArrow:
			Emit("\tcmp%c    %s, %s", Suf(), GetReg(left), GetReg(right));
			Emit("\tsetg    %s", GetReg(left, 1));
			Emit("\tmovz%c%c  %s, %s", Suf(1), Suf(), GetReg(left, 1), GetReg(left));
			goto Ret;
		case Token::Kind::RightArrowEqual:
			Emit("\tcmp%c    %s, %s", Suf(), GetReg(left), GetReg(right));
			Emit("\tsetge   %s", GetReg(left, 1));
			Emit("\tmovz%c%c  %s, %s", Suf(1), Suf(), GetReg(left, 1), GetReg(left));
			goto Ret;

		case Token::Kind::Ampersand:
			Emit("\tand%c    %s, %s", Suf(), GetReg(right), GetReg(left));
			goto Ret;
		case Token::Kind::Pipe:
			Emit("\tor%c     %s, %s", Suf(), GetReg(right), GetReg(left));
			goto Ret;
		case Token::Kind::AmpersandAmpersand:
			Emit("\ttest%c   %s, %s", Suf(), GetReg(left), GetReg(left));
			Emit("\tsetne   %s", GetReg(left, 1));
			Emit("\ttest%c   %s, %s", Suf(), GetReg(right), GetReg(right));
			Emit("\tsetne   %s", GetReg(right, 1));
			Emit("\tand%c    %s, %s", Suf(1), GetReg(right, 1), GetReg(left, 1));
			Emit("\tmovz%c%c  %s, %s", Suf(1), Suf(), GetReg(left, 1), GetReg(left));
			goto Ret;
		case Token::Kind::PipePipe:
			Emit("\ttest%c   %s, %s", Suf(), GetReg(left), GetReg(left));
			Emit("\tsetne   %s", GetReg(left, 1));
			Emit("\ttest%c   %s, %s", Suf(), GetReg(right), GetReg(right));
			Emit("\tsetne   %s", GetReg(right, 1));
			Emit("\tor%c   %s, %s", Suf(1), GetReg(right, 1), GetReg(left, 1));
			Emit("\tmovz%c%c  %s, %s", Suf(1), Suf(), GetReg(left, 1), GetReg(left));
			goto Ret;
		}

		return nullreg;

	Ret:
		mRegisterManager->Free(right);
		return left;
	}


	Register Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression)
	{
		if (expression->Is<NullLiteralExpressionNode>())
			return EmitNullLiteralExpression(expression->Cast<NullLiteralExpressionNode>());
		if (expression->Is<NumberLiteralExpressionNode>())
			return EmitNumberLiteralExpression(expression->Cast<NumberLiteralExpressionNode>());
		if (expression->Is<StringLiteralExpressionNode>())
			return EmitStringLiteralExpression(expression->Cast<StringLiteralExpressionNode>());
		if (expression->Is<BooleanLiteralExpressionNode>())
			return EmitBooleanLiteralExpression(expression->Cast<BooleanLiteralExpressionNode>());
		if (expression->Is<CharacterLiteralExpressionNode>())
			return EmitCharacterLiteralExpression(expression->Cast<CharacterLiteralExpressionNode>());

		return nullreg;
	}

	Register Emitter::EmitNullLiteralExpression(const NullLiteralExpressionNode*)
	{
		Register reg = mRegisterManager->Alloc();

		Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));

		return reg;
	}

	Register Emitter::EmitNumberLiteralExpression(const NumberLiteralExpressionNode* expression)
	{
		Register reg = mRegisterManager->Alloc();

		if (expression->Evaluate())
			Emit("\tmov%c    $%d, %s", Suf(), expression->Evaluate(), GetReg(reg));
		else
			Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));

		return reg;
	}

	Register Emitter::EmitStringLiteralExpression(const StringLiteralExpressionNode* expression)
	{
		Register reg = mRegisterManager->Alloc();

		Emit("\tmov%c    $..%i, %s", Suf(), mData, GetReg(reg));

		EmitLiteral("..%i:", mData++);
		EmitLiteral("\t.string  \"%s\"", std::string(expression->GetLiteral()->GetLex()).c_str());

		return reg;
	}

	Register Emitter::EmitBooleanLiteralExpression(const BooleanLiteralExpressionNode* expression)
	{
		Register reg = mRegisterManager->Alloc();

		if (expression->Evaluate())
			Emit("\tmov%c    $1, %s", Suf(), GetReg(reg));
		else
			Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));

		return reg;
	}

	Register Emitter::EmitCharacterLiteralExpression(const CharacterLiteralExpressionNode* expression)
	{
		Register reg = mRegisterManager->Alloc();

		if (expression->Evaluate())
			Emit("\tmov%c    $%i, %s", Suf(), expression->Evaluate(), GetReg(reg));
		else
			Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));

		return reg;
	}

	bool Emitter::OpenFile()
	{
		errno_t err = fopen_s(&mFile, mPath, "w");
		if (err && !mFile)
		{
			char msg[32];
			if (!strerror_s(msg, err))
			{
				char realMsg[56];
				sprintf_s(realMsg, "[!]: Error opening temp file: %.25s", msg);
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, realMsg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
			else
			{
				char msg[37] = "[!]: Unknown error opening temp file";
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, msg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	bool Emitter::OpenLiteralFile()
	{
		errno_t err = tmpfile_s(&mLiteralFile);
		if (err && !mLiteralFile)
		{
			char msg[32];
			if (!strerror_s(msg, err))
			{
				char realMsg[56];
				sprintf_s(realMsg, "[!]: Error opening temp file: %.25s", msg);
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, realMsg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
			else
			{
				char msg[37] = "[!]: Unknown error opening temp file";
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, msg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
		}
		else
		{
			return false;
		}

		return true;
	}
}