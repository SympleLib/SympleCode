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
		switch (sz)
		{
		case 1:
			return 'b';
		case 2:
			return 'w';
		case 4:
			return 'l';
		case 8:
			return 'q';
		}

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

		Emit("%s:", name);

		EmitBlockStatement(member->GetBody());

		Emit("\tret");
	}


	void Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<ExpressionStatementNode>())
			EmitExpressionStatement(statement->Cast<ExpressionStatementNode>());
	}

	void Emitter::EmitBlockStatement(const BlockStatementNode* block)
	{
		Emit("\tpushl   %%ebp");
		Emit("\tmovl    %%esp, %%ebp");
		if (block->GetStackUsage())
			Emit("\tsubl    $%i, %%esp", block->GetStackUsage());

		Debug::BeginScope();

		for (const StatementNode* statement : block->GetStatements())
			EmitStatement(statement);

		Debug::EndScope();

		Emit("\tmovl    %%ebp, %%esp");
		Emit("\tpopl    %%ebp");
	}

	void Emitter::EmitExpressionStatement(const ExpressionStatementNode* statement)
	{
		EmitExpression(statement->GetExpression());
	}

	Register Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>());
		
		return nullreg;
	}

	Register Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression)
	{
		if (expression->Is<NumberLiteralExpressionNode>())
			return EmitNumberLiteralExpression(expression->Cast<NumberLiteralExpressionNode>());

		return nullreg;
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