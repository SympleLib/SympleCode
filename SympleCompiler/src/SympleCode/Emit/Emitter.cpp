#include "SympleCode/Emit/Emitter.h"

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "SympleCode/Common/Node/GlobalStatementNode.h"
#include "SympleCode/Common/Node/ExpressionStatementNode.h"
#include "SympleCode/Common/Node/NumberLiteralExpressionNode.h"

#define Write(fmt, ...) (void)fprintf_s(mFile, fmt "\n", __VA_ARGS__);

namespace Symple
{
	Emitter::Emitter(const char* path)
		: mPath(path), mOpen(false)
	{
		while (OpenFile());
	}

	Emitter::~Emitter()
	{
		if (mOpen)
			fclose(mFile);
		mOpen = false;
	}

	void Emitter::Emit(const CompilationUnitNode* unit)
	{
		for (const MemberNode* member : unit->GetMembers())
			EmitMember(member);
	}

	void Emitter::EmitMember(const MemberNode* member)
	{
		if (member->Is<GlobalStatementNode>())
			return EmitStatement(member->Cast<GlobalStatementNode>()->GetStatement());
	}

	void Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<ExpressionStatementNode>())
			return EmitExpression(statement->Cast<ExpressionStatementNode>()->GetExpression());
	}

	void Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->Is<BinaryExpressionNode>())
			return EmitBinaryExpression(expression->Cast<BinaryExpressionNode>());
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>());
	}

	void Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression)
	{
		if (expression->Is<NumberLiteralExpressionNode>())
			Write("\tmovl $%s, %%eax", std::string(expression->GetLiteral()->GetLex()).c_str());
	}

	void Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression)
	{
		EmitExpression(expression->GetLeft());
		Write("\tmovl %%eax, %%edx");
		EmitExpression(expression->GetRight());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Plus:
			return Write("\taddl %%edx, %%eax");
		case Token::Kind::Minus:
			return Write("\tsubl %%edx, %%eax");
		case Token::Kind::Slash:
			return Write("\tdivl %%edx, %%eax");
		case Token::Kind::Asterisk:
			return Write("\tmull %%edx, %%eax");
		}
	}

	bool Emitter::OpenFile()
	{
		errno_t err = fopen_s(&mFile, mPath, "w");
		if (err)
		{
			char msg[32];
			if (!strerror_s(msg, err))
			{
				char realMsg[157];
				sprintf_s(realMsg, "[!]: Error opening file '%.100s%.3s': %.25s", mPath, strlen(mPath) > 100 ? "..." : "", msg);
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
				char msg[138];
				sprintf_s(msg, "[!]: Unknown error opening file '%.100s%.3s'", mPath, strlen(mPath) > 100 ? "..." : "");
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
			mOpen = true;
			return false;
		}

		return true;
	}
}