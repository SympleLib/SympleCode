#include "SympleCode/Emit/Emitter.h"

#include <Windows.h>

#define Emit(fmt, ...) fprintf(mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__)

#define GetReg(reg, ...) RegisterManager::GetRegister(reg, __VA_ARGS__)

namespace Symple
{
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


	void Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<BlockStatementNode>())
			EmitBlockStatement(statement->Cast<BlockStatementNode>());
		if (statement->Is<ExpressionStatementNode>())
			EmitExpressionStatement(statement->Cast<ExpressionStatementNode>());
	}

	void Emitter::EmitBlockStatement(const BlockStatementNode* block, bool funcdecl)
	{
		if (block->GetStackUsage())
			Emit("\tsubl    $%i, %%esp", block->GetStackUsage());

		Debug::BeginScope();

		for (const StatementNode* statement : block->GetStatements())
			EmitStatement(statement);

		Debug::EndScope();

		mRegisterManager->FreeAll();
	}

	void Emitter::EmitReturnStatement(const ReturnStatementNode* statement)
	{
		Register reg = mRegisterManager->Alloc(regax);

		Register exprReg = EmitExpression(statement->GetExpression());
		Emit("\tmovl    %s, %s", GetReg(exprReg), GetReg(reg));
		mRegisterManager->Free(exprReg);

		mRegisterManager->Free(reg);

		if (mReturning)
			Emit("\tjmp     ..%i", mReturn);
	}

	void Emitter::EmitExpressionStatement(const ExpressionStatementNode* statement)
	{
		Register exprReg = EmitExpression(statement->GetExpression());
		Register reg = mRegisterManager->Alloc(regax);
		Emit("\tmovl    %s, %s", GetReg(exprReg), GetReg(reg));
		mRegisterManager->Free(exprReg);

		mRegisterManager->Free(reg);
	}

	Register Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>());
		if (expression->Is<OperatorExpressionNode>())
			return EmitOperatorExpression(expression->Cast<OperatorExpressionNode>());
		if (expression->Is<ModifiableExpressionNode>())
			return EmitModifiableExpression(expression->Cast<ModifiableExpressionNode>());
		if (expression->Is<FunctionCallExpressionNode>())
			return EmitFunctionCallExpression(expression->Cast<FunctionCallExpressionNode>());
		
		return nullreg;
	}

	Register Emitter::EmitFunctionCallExpression(const FunctionCallExpressionNode* expression)
	{
		Register reg = nullreg;

		for (int i = 0; i < NumRegisters; i++)
			if (!mRegisterManager->GetFree()[i])
			{
				if (i == regax)
				{
					reg = mRegisterManager->Alloc();
					mRegisterManager->Free(i);
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
			if (i != reg && !mRegisterManager->GetFree()[i])
				Pop(i);

		return mRegisterManager->Alloc(regax);
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

		Register reg = mRegisterManager->Alloc();

		Emit("\tadd%c    $%i, %s", Suf(), off, GetReg(callee));
		mRegisterManager->Free(callee);

		if (retptr)
			return reg;

		Register val = mRegisterManager->CAlloc();
		unsigned int sz = expression->GetType()->GetSize();
		Emit("\tmov%c    (%s), %s", Suf(sz), GetReg(reg), GetReg(val, sz));
		return val;
	}

	Register Emitter::EmitVariableExpression(const VariableExpressionNode* expression, bool retptr)
	{
		Register reg = mRegisterManager->Alloc();

		if (retptr)
			Emit("\tlea%c    _%s@(%%ebp), %s", Suf(), std::string(expression->GetName()->GetLex()).c_str(), GetReg(reg));
		else
		{
			Emit("\txor%c    %s, %s", Suf(), GetReg(reg), GetReg(reg));

			const VariableDeclaration* var = Debug::GetVariable(expression->GetName()->GetLex());
			unsigned int sz = var->GetType()->GetSize();
			Emit("\tmov%c    _%s@(%%ebp), %s", Suf(sz), std::string(expression->GetName()->GetLex()).c_str(), GetReg(reg, sz));
		}

		return reg;
	}

	Register Emitter::EmitAssignmentExpression(const AssignmentExpressionNode* expression, bool retptr)
	{
		unsigned int sz = expression->GetLeft()->GetType()->GetSize();
		Register left = EmitModifiableExpression(expression->GetLeft(), true);
		Register right = EmitExpression(expression->GetRight());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Equal:
			Emit("\tmov%c    %s, (%s)", Suf(sz), GetReg(right, sz), GetReg(left));
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
		Emit("\tmov%c    (%s), %s", Suf(sz), GetReg(left), GetReg(right));
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
			Register out = mRegisterManager->CAlloc();

			Emit("\tcmp%c    $0, %s", Suf(), GetReg(reg));
			mRegisterManager->Free(reg);
			Emit("\tsete    %s", GetReg(out, 1));
			return out;
		case Token::Kind::Minus:
			Emit("\tneg%c    %s", Suf(), GetReg(reg));
			return reg;
		}
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
		}

	Ret:
		mRegisterManager->Free(right);
		return left;
	}


	Register Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression)
	{
		if (expression->Is<NumberLiteralExpressionNode>())
			return EmitNumberLiteralExpression(expression->Cast<NumberLiteralExpressionNode>());
		if (expression->Is<StringLiteralExpressionNode>())
			return EmitStringLiteralExpression(expression->Cast<StringLiteralExpressionNode>());

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