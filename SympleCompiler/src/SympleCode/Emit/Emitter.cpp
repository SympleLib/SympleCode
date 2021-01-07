#include "SympleCode/Emit/Emitter.h"

#include <Windows.h>

#define Emit(fmt, ...) fprintf(mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__)

#define FORMAT_MAX 16

namespace Symple
{
	static Emit RegAx = { nullptr, "%eax" };
	static Emit RegDx = { nullptr, "%edx" };
	static Emit RegBp = { nullptr, "%ebp" };
	static Emit RegSp = { nullptr, "%esp" };

	Emitter::Emitter()
		: mFile(), mData(), mReturn(),
			mReturning()
	{
		while (OpenFile());
	}

	Emit Emitter::EmitCompilationUnit(const CompilationUnitNode* unit)
	{
		for (const MemberNode* member : unit->GetMembers())
			EmitMember(member);

		return {};
	}

	static char* Format(const char* fmt, ...)
	{
		char* out = new char[FORMAT_MAX];

		va_list vl;
		va_start(vl, fmt);
		vsprintf_s(out, FORMAT_MAX, fmt, vl);
		va_end(vl);

		return out;
	}


	Emit Emitter::Push(Emit emit)
	{
		Emit("\tpushl   %s", emit.Eval);

		return {};
	}

	Emit Emitter::Pop(Emit emit)
	{
		Emit("\tpopl    %s", emit.Eval);

		return {};
	}

	Emit Emitter::Move(Emit from, Emit to)
	{
		if (from != to)
			if (from.Node && !from.Node->Evaluate())
				Emit("\txorl    %s, %s", to.Eval, to.Eval);
			else
				Emit("\tmovl    %s, %s", from.Eval, to.Eval);

		return { nullptr, to.Eval };
	}


	Emit Emitter::EmitMember(const MemberNode* member)
	{
		if (member->Is<FunctionDeclarationNode>())
			return EmitFunctionDeclaration(member->Cast<FunctionDeclarationNode>());

		return {};
	}

	Emit Emitter::EmitFunctionDeclaration(const FunctionDeclarationNode* declaration)
	{
		std::string nameStr = declaration->GetAsmName();
		const char* name = nameStr.c_str();

		Emit(".global %s", name);
		Emit("%s:", name);
		Push(RegBp);
		Move(RegSp, RegBp);

		mReturning = false;

		for (const StatementNode* statement : declaration->GetBody()->GetStatements())
			if (statement != declaration->GetBody()->GetStatements().back() && statement->Is<ReturnStatementNode>())
				mReturning = true;

		if (mReturning)
			mReturn = mData++;

		for (const StatementNode* statement : declaration->GetBody()->GetStatements())
			EmitStatement(statement);

		if (mReturning)
			Emit("..%i:", mReturn);
		Emit("\tleave");
		Emit("\tret");

		return { nullptr, name };
	}

	Emit Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<ReturnStatementNode>())
			return EmitReturnStatement(statement->Cast<ReturnStatementNode>());
		if (statement->Is<ExpressionStatementNode>())
			return EmitExpressionStatement(statement->Cast<ExpressionStatementNode>());

		return {};
	}

	Emit Emitter::EmitReturnStatement(const ReturnStatementNode* statement)
	{
		Move(EmitExpression(statement->GetExpression()), RegAx);
		if (mReturning)
			Emit("\tjmp     ..%i", mReturn);

		return {};
	}

	Emit Emitter::EmitExpressionStatement(const ExpressionStatementNode* statement)
	{
		return EmitExpression(statement->GetExpression());
	}

	Emit Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>());

		return { expression };
	}

	Emit Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression)
	{
		if (expression->Is<NullLiteralExpressionNode>())
			return EmitNullLiteralExpression(expression->Cast<NullLiteralExpressionNode>());
		if (expression->Is<NumberLiteralExpressionNode>())
			return EmitNumberLiteralExpression(expression->Cast<NumberLiteralExpressionNode>());
		if (expression->Is<StringLiteralExpressionNode>())
			return EmitStringLiteralExpression(expression->Cast<StringLiteralExpressionNode>());
		if (expression->Is<BooleanLiteralExpressionNode>())
			return EmitBooleanLiteralExpression(expression->Cast<BooleanLiteralExpressionNode>());
		if (expression->Is<NumberLiteralExpressionNode>())
			return EmitCharacterLiteralExpression(expression->Cast<CharacterLiteralExpressionNode>());

		return { expression };
	}

	Emit Emitter::EmitNullLiteralExpression(const NullLiteralExpressionNode* expression)
	{
		return { expression, "$0" };
	}

	Emit Emitter::EmitNumberLiteralExpression(const NumberLiteralExpressionNode* expression)
	{
		return { expression, Format("$%i", expression->Evaluate()) };
	}

	Emit Emitter::EmitStringLiteralExpression(const StringLiteralExpressionNode* expression)
	{
		EmitLiteral("..%i:", mData);
		EmitLiteral("\t.string \"%s\"", std::string(expression->GetLiteral()->GetLex()).c_str());

		return { expression, Format("$..%i", mData++) };
	}

	Emit Emitter::EmitBooleanLiteralExpression(const BooleanLiteralExpressionNode* expression)
	{
		return { expression, Format("$%i", expression->GetLiteral()->Is(Token::Kind::True)), 1 };
	}

	Emit Emitter::EmitCharacterLiteralExpression(const CharacterLiteralExpressionNode* expression)
	{
		return { expression, Format("$%i", expression->GetLiteral()->GetLex()[0]), 1 };
	}

	bool Emitter::OpenFile()
	{
		errno_t err = fopen_s(&mFile, "bin/out.s", "w");
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