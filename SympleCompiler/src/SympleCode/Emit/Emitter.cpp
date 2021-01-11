#include "SympleCode/Emit/Emitter.h"

#include <Windows.h>

#define Emit(fmt, ...) fprintf(mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__)

#define GetReg RegisterManager::GetRegister

namespace Symple
{
	Emitter::Emitter(const char* path)
		: mPath(path), mFile(), mLiteralFile(), mData(), mReturn(), mStack(), mInits(),
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

	char Emitter::Suf(int sz)
	{
		if (sz <= 1)
			return 'b';
		if (sz <= 2)
			return 'w';
		if (sz <= 4)
			return 'l';
		//if (sz <= 8)
		//	return 'q';

		return 0;
	}

	void Emitter::Push(Register reg, int sz)
	{
		Emit("\tpush%c   %s", Suf(sz), GetReg(reg, sz));
	}

	void Emitter::Pop(Register reg, int sz)
	{
		Emit("\tpop%c    %s", Suf(sz), GetReg(reg, sz));
	}

	void Emitter::EmitMember(const MemberNode* member)
	{
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

		const char* name = member->GetAsmName().c_str();

		if (member->GetModifiers()->IsPrivate())
			Emit("\t.local   %s .type %s, @function", name, name);
		else
			Emit("\t.globl   %s", name);

		int argOff = 4;
		for (const FunctionArgumentNode* arg : member->GetArguments()->GetArguments())
		{
			Emit("_%s$ = %i", std::string(arg->GetName()->GetLex()).c_str(), argOff);
			argOff += 4;
		}

		mStack = 0;

		Emit("%s:", name);
		Emit("\tpushl   %%ebp");
		Emit("\tmovl    %%esp, %%ebp");

		if (member->IsMain())
			for (unsigned int i = 0; i < mInits; i++)
				Emit("\tcall%c   ..%i.", Suf(), i);

		mReturning = false;
		for (const StatementNode* statement : member->GetBody()->GetStatements())
			if (statement->Is<ReturnStatementNode>() && statement != member->GetBody()->GetStatements().back())
				mReturning = true;

		if (mReturning)
			mReturn = mData++;

		EmitBlockStatement(member->GetBody());

		if (mReturning)
			Emit("..%i:", mReturn);
		Emit("\tmovl    %%ebp, %%esp");
		Emit("\tpopl    %%ebp");
		Emit("\tret");
	}

	void Emitter::EmitGlobalVariableDeclaration(const GlobalVariableDeclarationNode* member)
	{
		std::string nstr(member->GetName()->GetLex());
		const char* name = nstr.c_str();

		Emit("\t.bss");
		if (member->GetModifiers()->IsPrivate())
			Emit("\t.local   _%s@", name);
		else
			Emit("\t.globl   _%s@", name);
		Emit("_%s@:", name);
		Emit("\t.zero %i", member->GetType()->GetSize());
		Emit("\t.text");

		if (member->GetInitializer())
		{
			Emit("..%i.:", mInits++);
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


	void Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<IfStatementNode>())
			EmitIfStatement(statement->Cast<IfStatementNode>());
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

			EmitBlockStatement(statement->GetElse());
		}
	}

	void Emitter::EmitBlockStatement(const BlockStatementNode* block, bool funcdecl)
	{
		if (block->GetStackUsage())
		{
			Emit("\tsubl    $%i, %%esp", block->GetStackUsage());
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
		if (statement->GetInitializer())
		{
			if (statement->GetInitializer()->Is<StructInitializerExpressionNode>())
			{
				Register ptr = mRegisterManager->Alloc();
				Emit("\tlea%c    _%s@(%%ebp), %s", Suf(), name, GetReg(ptr));
				EmitStructInitializerExpression(statement->GetInitializer()->Cast<StructInitializerExpressionNode>(), ptr);
				mRegisterManager->Free(ptr);
			}
			else
			{
				Register init = EmitExpression(statement->GetInitializer());
				Emit("\tmov%c    %s, _%s@(%%ebp)", Suf(), GetReg(init), name);
				mRegisterManager->Free(init);
			}
		}

		if (statement->GetNext())
			EmitVariableDeclaration(statement->GetNext());
	}


	Register Emitter::EmitExpression(const ExpressionNode* expression)
	{
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
		Emit("\tsub%c   $%i, %%esp", Suf(), sz);

		mStack += sz;
		for (const ExpressionNode* item : expression->GetExpressions())
		{
			Register itemReg = EmitExpression(item);
			Emit("\tmov%c    %s, -%i(%%ebp)", Suf(), GetReg(itemReg), mStack);
			mRegisterManager->Free(itemReg);

			mStack -= item->GetType()->GetSize();
		}
		mStack += sz;

		Emit("\tlea%c    -%i(%%ebp), %s", Suf(), mStack, GetReg(reg));
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
			Emit("\tmov%c    %s, %s", Suf(), GetReg(then), GetReg(reg));
			mRegisterManager->Free(then);
		}

		mRegisterManager->Free(reg);
		Emit("\tjmp     ..%i", end);
		Emit("..%i:", elze);

		Register elzeReg = EmitExpression(expression->GetElse());
		if (elzeReg != reg)
		{
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
					Emit("\tmov%c    %%eax, %s", Suf(), GetReg(reg));
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

		Emit("\tcalll   %s", function->GetAsmName().c_str());
		Emit("\taddl    $%i, %%esp", expression->GetArguments()->GetArguments().size() * 4);

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
		return EmitVariableExpression(expression->GetVariable(), true);
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

		Emit("\tadd%c    $%i, %s", Suf(), off, GetReg(callee));

		if (retptr)
			return callee;

		Register val = mRegisterManager->CAlloc();
		unsigned int sz = expression->GetType()->GetSize();
		Emit("\tmov%c    (%s), %s", Suf(sz), GetReg(callee), GetReg(val, sz));
		mRegisterManager->Free(callee);
		return val;
	}

	Register Emitter::EmitVariableExpression(const VariableExpressionNode* expression, bool retptr)
	{
		const VariableDeclaration* var = Debug::GetVariable(expression->GetName()->GetLex());
		Register reg = mRegisterManager->Alloc();

		if (var->Is<VariableDeclarationNode>())
		{
			if (retptr)
				Emit("\tlea%c    _%s@(%%ebp), %s", Suf(), std::string(expression->GetName()->GetLex()).c_str(), GetReg(reg));
			else
			{
				unsigned int sz = var->GetType()->GetSize();

				if (sz != 4)
					Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));
				Emit("\tmov%c    _%s@(%%ebp), %s", Suf(sz), std::string(expression->GetName()->GetLex()).c_str(), GetReg(reg, sz));
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
		}

		return nullreg;

	Ret:
		if (retptr)
		{
			mRegisterManager->Free(right);
			return left;
		}

		Register reg = mRegisterManager->Alloc();
		Emit("\tmov%c    (%s), %s", Suf(sz), GetReg(left), GetReg(reg));
		mRegisterManager->Free(left);
		mRegisterManager->Free(right);
		return reg;
	}

	Register Emitter::EmitPointerIndexExpression(const PointerIndexExpressionNode* expression, bool retptr)
	{
		Register reg = EmitExpression(expression->GetAddress());
		Register off = EmitExpression(expression->GetIndex());

		Emit("\tadd%c    %s, %s", Suf(), GetReg(off), GetReg(reg));

		if (retptr)
			return reg;

		Register val = mRegisterManager->CAlloc();
		unsigned int sz = expression->GetType()->GetSize();
		Emit("\tmov%c    (%s), %s", Suf(sz), GetReg(reg), GetReg(val, sz));
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
		case Token::Kind::LeftArrow:
			Emit("\tcmp%c    %s, %s", Suf(), GetReg(right), GetReg(left));
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

		Emit("\tmovl    $..%i, %s", mData, GetReg(reg));

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