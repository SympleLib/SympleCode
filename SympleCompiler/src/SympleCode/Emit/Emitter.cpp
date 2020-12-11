#include "SympleCode/Emit/Emitter.h"

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "SympleCode/Common/Node/GlobalStatementNode.h"
#include "SympleCode/Common/Node/ExpressionStatementNode.h"
#include "SympleCode/Common/Node/NumberLiteralExpressionNode.h"

#define Write(fmt, ...) (void)fprintf_s(mFile, fmt, __VA_ARGS__);

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
	}

	void Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression)
	{
		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Plus:
			if (expression->GetLeft()->Is<BinaryExpressionNode>())
			{
				EmitBinaryExpression(expression->GetLeft()->Cast<BinaryExpressionNode>());
				if (expression->GetRight()->Is<BinaryExpressionNode>())
				{
					Write("movl %%eax, %%edx");
					EmitBinaryExpression(expression->GetRight()->Cast<BinaryExpressionNode>());
					Write("addl %%edx, %%eax");
				}
				else
				{
					Write("addl %s, %%eax", std::string(expression->GetLeft()->Cast<NumberLiteralExpressionNode>()->GetLiteral()->GetLex()).c_str());
				}
			}
			else if (expression->GetRight()->Is<BinaryExpressionNode>())
			{
				EmitBinaryExpression(expression->GetRight()->Cast<BinaryExpressionNode>());

				if (expression->GetLeft()->Is<BinaryExpressionNode>())
				{
					Write("movl %%eax, %%edx");
					EmitBinaryExpression(expression->GetLeft()->Cast<BinaryExpressionNode>());
					Write("addl %%edx, %%eax");
				}
				else
				{
					Write("addl %s, %%eax", std::string(expression->GetRight()->Cast<NumberLiteralExpressionNode>()->GetLiteral()->GetLex()).c_str());
				}
			}
			break;
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