#include "SympleCode/Emit/Emitter.h"

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "SympleCode/Common/Node/Member/FunctionHintNode.h"
#include "SympleCode/Common/Node/Member/ExternFunctionNode.h"
#include "SympleCode/Common/Node/Member/GlobalStatementNode.h"

#include "SympleCode/Common/Node/Statement/ExpressionStatementNode.h"

#include "SympleCode/Common/Node/Expression/ParenthesizedExpressionNode.h"
#include "SympleCode/Common/Node/Expression/NumberLiteralExpressionNode.h"
#include "SympleCode/Common/Node/Expression/BooleanLiteralExpressionNode.h"

#if DO_COMMENTS
#define Comment(fmt, ...) (void)fprintf_s(mFile, "# " fmt "\n", __VA_ARGS__)
#else
#define Comment(fmt, ...) (void())
#endif
#define Write(fmt, ...) ((void)fprintf_s(mFile, fmt "\n", __VA_ARGS__))
#define WriteLiteral(fmt, ...) ((void)fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__))

namespace Symple
{
	Emitter::Emitter(Diagnostics* diagnostics, const char* path)
		: mDiagnostics(diagnostics), mPath(path), mStackPos(0), mStringPos(0), mJumpPos(0), mDeclaredVariables()
	{
		while (OpenFile());
		while (OpenLiteralFile());
	}

	Emitter::~Emitter()
	{
		rewind(mLiteralFile);

		char c;
		while ((c = fgetc(mLiteralFile)) != EOF)
			fputc(c, mFile);

		fclose(mLiteralFile);
		fclose(mFile);
	}

	void Emitter::Emit(const CompilationUnitNode* unit)
	{
		for (const MemberNode* member : unit->GetMembers())
			EmitMember(member);
	}

	char* Emitter::RegAx(int size)
	{
		if (size <= 1)
			return "%al";
		if (size <= 2)
			return "%ax";
		if (size <= 4)
			return "%eax";
		return "%eax";
	}

	char* Emitter::RegDx(int size)
	{
		if (size <= 1)
			return "%dl";
		if (size <= 2)
			return "%dx";
		if (size <= 4)
			return "%edx";
		return "%eax";
	}

	char* Emitter::RegCx()
	{
		return "%ecx";
	}

	char* Emitter::RegSp()
	{
		return "%esp";
	}

	char* Emitter::RegBp()
	{
		return "%ebp";
	}

	char Emitter::Mod(int size)
	{
		if (size <= 1)
			return 'b';
		if (size <= 2)
			return 'w';
		if (size <= 4)
			return 'l';
		return ' ';
	}

	void Emitter::EmitCast(int from, int to)
	{
		if (from == to)
			return;
		return Write("\tmovs%c%c  %s, %s", Mod(from), Mod(to), RegAx(from), RegAx(to));
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
		if (declaration->Is<FunctionHintNode>())
			return;
		if (declaration->Is<ExternFunctionNode>())
			return Write(".extern _%s", std::string(declaration->GetName()->GetLex()).c_str());

		unsigned int pStackPos = mStackPos;
		mStackPos = 0;

		std::string name = std::string(declaration->GetName()->GetLex());
		Comment("Function Declaration");
		if (name[0] != '_')
			Write(".global _%s", name.c_str());
		Write("_%s:", name.c_str());
		Comment("\tPush Stack");
		Write("\tpush    %s", RegBp());
		Write("\tmovl    %s, %s", RegSp(), RegBp());
		if (name == "main")
			Write("\txorl    %%eax, %%eax");

		Comment("\tFunction Arguments");
		unsigned int pos = 4;
		for (const FunctionArgumentNode* argument : declaration->GetArguments()->GetArguments())
		{
			pos += 4;
			Write("_%s$ = %i", std::string(argument->GetName()->GetLex()).c_str(), pos);
			mDeclaredVariables.insert({ argument->GetName()->GetLex(), new VariableDeclarationNode(argument->GetName(), argument->GetType(), nullptr) });
		}

		for (const StatementNode* statement : declaration->GetBody()->GetStatements())
			EmitStatement(statement);

		Comment("\tPop Stack and Return");
		Write("\tmovl    %s, %s", RegBp(), RegSp());
		Write("\tpop     %s", RegBp());
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
		unsigned int size = declaration->GetType()->GetSize();
		mStackPos += size;

		mDeclaredVariables.insert({ declaration->GetName()->GetLex(), declaration });

		Write("_%s$ = -%i", name.c_str(), mStackPos);
		Write("\tsubl    $%i, %s", size, RegSp());
		if (declaration->GetInitializer()->GetKind() != Node::Kind::Expression)
		{
			EmitExpression(declaration->GetInitializer(), size);
			Write("\tmov%c    %s, _%s$(%s)", Mod(size), RegAx(size), name.c_str(), RegBp());
		}
	}

	void Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<ExpressionStatementNode>())
			return EmitExpression(statement->Cast<ExpressionStatementNode>()->GetExpression());
		if (statement->Is<ReturnStatementNode>())
			EmitExpression(statement->Cast<ReturnStatementNode>()->GetExpression());
		if (statement->Is<IfStatementNode>())
			return EmitIfStatement(statement->Cast<IfStatementNode>());
		if (statement->Is<WhileStatementNode>())
			return EmitWhileStatement(statement->Cast<WhileStatementNode>());
		if (statement->Is<VariableDeclarationNode>())
			return EmitVariableDeclaration(statement->Cast<VariableDeclarationNode>());
		if (statement->Is<BlockStatementNode>())
			return EmitBlockStatement(statement->Cast<BlockStatementNode>());
		if (statement->Is<BreakStatementNode>())
			return EmitBreakStatement(statement->Cast<BreakStatementNode>());
	}

	void Emitter::EmitIfStatement(const IfStatementNode* statement)
	{
		unsigned int afterJumpPos = mJumpPos;
		mJumpPos++;
		unsigned int elseJumpPos = mJumpPos;
		mJumpPos++;


		EmitExpression(statement->GetCondition());
		Write("\tcmp%c    $0, %s", Mod(), RegAx());
		Write("\tje      ..Jump.%i", elseJumpPos);

		for (const StatementNode* statement : statement->GetThen()->GetStatements())
			EmitStatement(statement);

		Write("\tjmp     ..Jump.%i", afterJumpPos);
		Write("..Jump.%i:", elseJumpPos);

		for (const StatementNode* statement : statement->GetElse()->GetStatements())
			EmitStatement(statement);

		Write("..Jump.%i:", afterJumpPos);
	}

	void Emitter::EmitBreakStatement(const BreakStatementNode* statement)
	{
		if (!mJumpPos)
			return mDiagnostics->ReportError(statement->GetStatement(), "There is Nothing to Break");
		Write("\tjmp     ..Jump.%i", mJumpPos - 1);
	}

	void Emitter::EmitBlockStatement(const BlockStatementNode* statement)
	{
		for (const StatementNode* statement : statement->GetStatements())
			EmitStatement(statement);

		for (const auto& variable : statement->GetVariables())
			mDeclaredVariables.erase(mDeclaredVariables.find(variable.first));
	}

	void Emitter::EmitWhileStatement(const WhileStatementNode* statement)
	{
		unsigned int pJumpPos = mJumpPos;
		mJumpPos++;
		unsigned int aJumpPos = mJumpPos;
		mJumpPos++;

		Write("..Jump.%i:", pJumpPos);

		EmitExpression(statement->GetCondition());
		Write("\tcmp%c    $0, %s", Mod(), RegAx());
		Write("\tje      ..Jump.%i", aJumpPos);

		for (const StatementNode* statement : statement->GetBody()->GetStatements())
			EmitStatement(statement);

		Write("\tjmp     ..Jump.%i", pJumpPos);
		Write("..Jump.%i:", aJumpPos);
	}

	void Emitter::EmitExpression(const ExpressionNode* expression, int size)
	{
		Write("");
		if (expression->Is<ParenthesizedExpressionNode>())
			return EmitExpression(expression->Cast<ParenthesizedExpressionNode>()->GetExpression(), size);
		if (expression->Is<UnaryExpressionNode>())
			return EmitUnaryExpression(expression->Cast<UnaryExpressionNode>(), size);
		if (expression->Is<BinaryExpressionNode>())
			return EmitBinaryExpression(expression->Cast<BinaryExpressionNode>(), size);
		if (expression->Is<FunctionCallExpressionNode>())
			return EmitFunctionCallExpression(expression->Cast<FunctionCallExpressionNode>(), size);
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>(), size);
		if (expression->Is<VariableExpressionNode>())
			return EmitVariableExpression(expression->Cast<VariableExpressionNode>(), size);
		if (expression->Is<PointerIndexExpressionNode>())
			return EmitPointerIndexExpression(expression->Cast<PointerIndexExpressionNode>(), size);
	}

	void Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression, int size)
	{
		if (expression->Is<StringLiteralExpressionNode>())
			return EmitStringLiteralExpression(expression->Cast<StringLiteralExpressionNode>());

		if (expression->Is<NumberLiteralExpressionNode>())
			Write("\tmov%c    $%s, %s", Mod(size), std::string(expression->GetLiteral()->GetLex()).c_str(), RegAx(size));
		if (expression->Is<BooleanLiteralExpressionNode>())
			Write("\tmov%c    $%i, %s", Mod(size), expression->GetLiteral()->Is(Token::Kind::True), RegAx(size));
	}

	void Emitter::EmitUnaryExpression(const UnaryExpressionNode* expression, int size)
	{
		Comment("Unary Expression [%s]", std::string(expression->GetOperator()->GetLex()).c_str());
		if (expression->GetOperator()->Is(Token::Kind::At))
		{
			if (expression->GetValue()->GetKind() == Node::Kind::VariableExpression)
			{
				std::string name(expression->GetValue()->Cast<VariableExpressionNode>()->GetName()->GetLex());
				return Write("\tlea%c    _%s$(%s), %s", Mod(mDeclaredVariables[name]->GetType()->GetSize()), name.c_str(), RegBp(), RegAx());
			}
			if (expression->GetValue()->GetKind() == Node::Kind::PointerIndexExpression)
			{
				const PointerIndexExpressionNode* expr = expression->GetValue()->Cast<PointerIndexExpressionNode>();
				std::string name(expr->GetExpression()->GetName()->GetLex());

				EmitExpression(expr->GetIndex());
				Write("\tmov%c    %s, %s", Mod(), RegAx(), RegDx());
				Write("\tlea%c    _%s$(%s), %s", Mod(mDeclaredVariables[name]->GetType()->GetSize()), name.c_str(), RegBp(), RegAx());
				return Write("\tadd%c    %s, %s", Mod(), RegDx(), RegAx());
			}

			return mDiagnostics->ReportError(expression->GetOperator(), "Expected lvalue");
		}
		EmitExpression(expression->GetValue());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Asterisk:
			Write("\tmov%c    (%s), %s", Mod(), RegAx(), RegAx());
			break;
		case Token::Kind::Minus:
			Write("\tneg%c    %s", Mod(), RegAx());
			break;
		}
	}

	void Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression, int size)
	{
		Comment("Binary Expression [%s]", std::string(expression->GetOperator()->GetLex()).c_str());
		if (expression->GetOperator()->Is(Token::Kind::Equal))
		{
			if (expression->GetLeft()->GetKind() == Node::Kind::VariableExpression)
			{
				EmitExpression(expression->GetRight(), size);
				return Write("\tmov%c    %s, _%s$(%s)", Mod(), RegAx(), std::string(expression->GetLeft()->Cast<VariableExpressionNode>()->GetName()->GetLex()).c_str(), RegBp());
			}
			if (expression->GetLeft()->GetKind() == Node::Kind::PointerIndexExpression)
			{
				const PointerIndexExpressionNode* expr = expression->GetLeft()->Cast<PointerIndexExpressionNode>();

				EmitExpression(expression->GetRight());
				Write("\tpush%c    %s", Mod(), RegAx());
				EmitExpression(expr->GetIndex());
				Write("\tpush%c   %s", Mod(), RegAx());
				Write("\tmov%c    _%s$(%s), %s", Mod(), std::string(expr->GetExpression()->GetName()->GetLex()).c_str(), RegBp(), RegAx());
				Write("\tpop%c    %s", Mod(), RegDx());
				Write("\tadd%c    %s, %s", Mod(), RegDx(), RegAx());
				Write("\tpop%c    %s", Mod(), RegDx());
				Write("\tmov%c    %s, (%s)", Mod(1), RegDx(1), RegAx());
				Write("\tmov%c    %s, %s", Mod(1), RegDx(1), RegAx(1));
				return EmitCast(1);
			}
			
			return mDiagnostics->ReportError(expression->GetOperator(), "Expected lvalue");
		}

		EmitExpression(expression->GetRight());
		Write("\tpush%c   %s", Mod(), RegAx());
		EmitExpression(expression->GetLeft());
		Write("\tpop%c    %s", Mod(), RegDx());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Plus:
			Write("\tadd%c    %s, %s", Mod(), RegDx(), RegAx());
			break;
		case Token::Kind::Minus:
			Write("\tsub%c    %s, %s", Mod(), RegDx(), RegAx());
			break;
		case Token::Kind::Asterisk:
			Write("\timu%c    %s, %s", Mod(), RegDx(), RegAx());
			break;
		case Token::Kind::EqualEqual:
		{
			unsigned int jumpPos = mJumpPos;
			mJumpPos++;

			Write("\tcmp%c    %s, %s", Mod(), RegDx(), RegAx());
			Write("\tmov%c    $1, %s", Mod(), RegAx());
			Write("\tje      ..Jump.%i", jumpPos);
			Write("\tmov%c    $0, %s", Mod(), RegAx());
			Write("..Jump.%i:", jumpPos);
			break;
		}
		case Token::Kind::NotEqual:
		{
			unsigned int jumpPos = mJumpPos;
			mJumpPos++;

			Write("\tcmp%c    %s, %s", Mod(), RegDx(), RegAx());
			Write("\tmov%c    $0, %s", Mod(), RegAx());
			Write("\tje      ..Jump.%i", jumpPos);
			Write("\tmov%c    $1, %s", Mod(), RegAx());
			Write("..Jump.%i:", jumpPos);
			break;
		}
		}
	}

	void Emitter::EmitVariableExpression(const VariableExpressionNode* expression, int size)
	{
		std::string name = std::string(expression->GetName()->GetLex());
		int varSize = mDeclaredVariables[name]->GetType()->GetSize();
		if (!VariableDefined(expression->GetName()->GetLex()))
			return mDiagnostics->ReportError(expression->GetName(), "'%s' is not a Variable", name.c_str());

		Write("\tmov%c    _%s$(%s), %s", Mod(varSize), name.c_str(), RegBp(), RegAx(varSize));
		EmitCast(varSize);
	}

	void Emitter::EmitFunctionCallExpression(const FunctionCallExpressionNode* call, int size)
	{
		const FunctionDeclarationNode* function = mDiagnostics->GetFunction(call->GetName()->GetLex());
		if (!function)
			return mDiagnostics->ReportError(call->GetName(), "Function does not exist!");

		Comment("Function Call");
		Comment("\tPush Arguments");

		unsigned int pushedSize = 0;
		for (int i = call->GetArguments()->GetArguments().size() - 1; i >= 0; i--)
		{
			EmitExpression(call->GetArguments()->GetArguments()[i]);
			Write("\tpush%c   %s", Mod(), RegAx());

			pushedSize += 4;
		}
		Comment("\tCall Function");
		Write("\tcall    _%s", std::string(call->GetName()->GetLex()).c_str());
		Comment("\tPop Arguments");
		Write("\taddl    $%i, %s", pushedSize, RegSp());
	}

	void Emitter::EmitPointerIndexExpression(const PointerIndexExpressionNode* expression, int size)
	{
		EmitExpression(expression->GetIndex());
		Write("\tmov%c    %s, %s", Mod(), RegAx(), RegDx());
		Write("\tmov%c    _%s$(%s), %s", Mod(), std::string(expression->GetExpression()->GetName()->GetLex()).c_str(), RegBp(), RegAx());
		Write("\tadd%c    %s, %s", Mod(), RegDx(), RegAx());
		Write("\tmov%c    (%s), %s", Mod(), RegAx(), RegAx());
	}

	void Emitter::EmitStringLiteralExpression(const StringLiteralExpressionNode* literal, int size)
	{
		WriteLiteral("..String.%i:", mStringPos);
		WriteLiteral("\t.string \"%s\"", std::string(literal->GetLiteral()->GetLex()).c_str());
		Write("\tmov%c    $..String.%i, %s", Mod(), mStringPos, RegAx());

		mStringPos++;
	}

	bool Emitter::VariableDefined(const std::string_view& name)
	{
		return mDeclaredVariables.find(name) != mDeclaredVariables.end();
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