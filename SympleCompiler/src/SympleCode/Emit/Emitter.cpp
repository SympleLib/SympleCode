#include "SympleCode/Emit/Emitter.h"

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "SympleCode/Common/Node/GlobalStatementNode.h"
#include "SympleCode/Common/Node/ExpressionStatementNode.h"
#include "SympleCode/Common/Node/NumberLiteralExpressionNode.h"
#include "SympleCode/Common/Node/BooleanLiteralExpressionNode.h"

#if DO_COMMENTS
#define Comment(fmt, ...) (void)fprintf_s(mFile, "# " fmt "\n", __VA_ARGS__)
#else
#define Comment(fmt, ...)
#endif
#define Write(fmt, ...) (void)fprintf_s(mFile, fmt "\n", __VA_ARGS__)

namespace Symple
{
	Emitter::Emitter(Diagnostics* diagnostics, const char* path)
		: mDiagnostics(diagnostics), mPath(path), mOpen(false), mStackPos(0), mDeclaredVariables()
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
		if (member->Is<FunctionDeclarationNode>())
			return EmitFunctionDeclaration(member->Cast<FunctionDeclarationNode>());
		if (member->Is<GlobalStatementNode>())
			return EmitStatement(member->Cast<GlobalStatementNode>()->GetStatement());
	}

	void Emitter::EmitFunctionDeclaration(const FunctionDeclarationNode* declaration)
	{
		unsigned int pStackPos = mStackPos;
		mStackPos = 0;

		Comment("Function Declaration");
		Write("%s:", std::string(declaration->GetName()->GetLex()).c_str());
		Comment("\tPush Stack");
		Write("\tpush    %%ebp");
		Write("\tmovl    %%esp, %%ebp");
		Write("\txorl    %%eax, %%eax");

		Comment("\tFunction Arguments");
		unsigned int pos = 4;
		for (const FunctionArgumentNode* argument : declaration->GetArguments()->GetArguments())
		{
			pos += argument->GetType()->GetSize();
			Write("_%s$ = %i", std::string(argument->GetName()->GetLex()).c_str(), pos);
			mDeclaredVariables.insert({ argument->GetName()->GetLex(), nullptr });
		}

		for (const StatementNode* statement : declaration->GetBody()->GetStatements())
			EmitStatement(statement);

		Comment("\tPop Stack and Return");
		Write("\tmovl    %%ebp, %%esp");
		Write("\tpop     %%ebp");
		Write("\tret");

		for (const auto& variable : declaration->GetBody()->GetVariables())
			mDeclaredVariables.erase(mDeclaredVariables.find(variable.first));
		for (const FunctionArgumentNode* argument : declaration->GetArguments()->GetArguments())
			mDeclaredVariables.erase(mDeclaredVariables.find(argument->GetName()->GetLex()));

		mStackPos = pStackPos;
	}

	void Emitter::EmitVariableDeclaration(const VariableDeclarationNode* declaration)
	{
		std::string name = std::string(declaration->GetName()->GetLex());
		mStackPos += declaration->GetType()->GetSize();

		mDeclaredVariables.insert({ declaration->GetName()->GetLex(), declaration });

		Write("_%s$ = -%i", name.c_str(), mStackPos);
		Write("\tsubl    $%i, %%esp", declaration->GetType()->GetSize());
		if (declaration->GetInitializer()->GetKind() != Node::Kind::Expression)
		{
			EmitExpression(declaration->GetInitializer());
			Write("\tmovl    %%eax, _%s$(%%ebp)", name.c_str());
		}
	}

	void Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<ExpressionStatementNode>())
			return EmitExpression(statement->Cast<ExpressionStatementNode>()->GetExpression());
		if (statement->Is<ReturnStatementNode>())
			EmitExpression(statement->Cast<ReturnStatementNode>()->GetExpression());
		if (statement->Is<VariableDeclarationNode>())
			return EmitVariableDeclaration(statement->Cast<VariableDeclarationNode>());
		if (statement->Is<BlockStatementNode>())
			return EmitBlockStatement(statement->Cast<BlockStatementNode>());
	}

	void Emitter::EmitBlockStatement(const BlockStatementNode* statement)
	{
		for (const StatementNode* statement : statement->GetStatements())
			EmitStatement(statement);

		for (const auto& variable : statement->GetVariables())
			mDeclaredVariables.erase(mDeclaredVariables.find(variable.first));
	}

	void Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->Is<BinaryExpressionNode>())
			return EmitBinaryExpression(expression->Cast<BinaryExpressionNode>());
		if (expression->Is<FunctionCallExpressionNode>())
			return EmitFunctionCallExpression(expression->Cast<FunctionCallExpressionNode>());
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>());
		if (expression->Is<VariableExpressionNode>())
			return EmitVariableExpression(expression->Cast<VariableExpressionNode>());
	}

	void Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression)
	{
		if (expression->Is<NumberLiteralExpressionNode>())
			Write("\tmovl    $%s, %%eax", std::string(expression->GetLiteral()->GetLex()).c_str());
		if (expression->Is<BooleanLiteralExpressionNode>())
			Write("\tmovl    $%i, %%eax", expression->GetLiteral()->Is(Token::Kind::True));
	}

	void Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression)
	{
		Comment("Binary Expression [%s]", std::string(expression->GetOperator()->GetLex()).c_str());
		if (expression->GetOperator()->Is(Token::Kind::Equal))
		{
			if (expression->GetLeft()->GetKind() != Node::Kind::VariableExpression)
			{
				mDiagnostics->ReportError(expression->GetOperator(), "Expected lvalue");
				return;
			}

			EmitExpression(expression->GetRight());
			Write("\tmovl    %%eax, _%s$(%%ebp)", std::string(expression->GetLeft()->Cast<VariableExpressionNode>()->GetName()->GetLex()).c_str());
			return;
		}

		EmitExpression(expression->GetRight());
		Write("\tmovl    %%eax, %%edx");
		EmitExpression(expression->GetLeft());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Plus:
			Write("\taddl    %%edx, %%eax");
			break;
		case Token::Kind::Minus:
			Write("\tsubl    %%edx, %%eax");
			break;
		case Token::Kind::Asterisk:
			Write("\timul    %%edx, %%eax");
			break;
		}
	}

	void Emitter::EmitVariableExpression(const VariableExpressionNode* expression)
	{
		if (!VariableDefined(expression->GetName()->GetLex()))
			mDiagnostics->ReportError(expression->GetName(), "'%s' is not a Variable", std::string(expression->GetName()->GetLex()).c_str());

		Write("\tmovl    _%s$(%%ebp), %%eax", std::string(expression->GetName()->GetLex()).c_str());
	}

	void Emitter::EmitFunctionCallExpression(const FunctionCallExpressionNode* call)
	{
		Comment("Function Call");
		Comment("\tPush Arguments");
		for (int i = call->GetArguments()->GetArguments().size() - 1; i >= 0; i--)
		{
			EmitExpression(call->GetArguments()->GetArguments()[i]);
			Write("\tpush    %%eax");
		}
		Comment("\tCall Function");
		Write("\tcall    %s", std::string(call->GetName()->GetLex()).c_str());
		Comment("\tPop Arguments");
		Write("\taddl    $%i, %%esp", mDiagnostics->GetFunction(call->GetName()->GetLex())->GetArguments()->GetSize());
	}

	bool Emitter::VariableDefined(const std::string_view& name)
	{
		return mDeclaredVariables.find(name) != mDeclaredVariables.end();
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