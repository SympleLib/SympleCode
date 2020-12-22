#include "SympleCode/Emit/Emitter.h"

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <memory>

#include "SympleCode/Common/Node/Member/FunctionHintNode.h"
#include "SympleCode/Common/Node/Member/ExternFunctionNode.h"

#include "SympleCode/Common/Node/Expression/ParenthesizedExpressionNode.h"
#include "SympleCode/Common/Node/Expression/NumberLiteralExpressionNode.h"
#include "SympleCode/Common/Node/Expression/BooleanLiteralExpressionNode.h"

#define Write(fmt, ...) ((void)fprintf_s(mFile, fmt "\n", __VA_ARGS__))
#define WriteLiteral(fmt, ...) ((void)fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__))

#define RegErr "ERROR"

#define RegCx "%ecx"
#define RegSp "%esp"
#define RegBp "%ebp"

#define FORMAT__MAX 128

namespace Symple
{
	Emitter::Emitter(Diagnostics* diagnostics, const char* path)
		: mDiagnostics(diagnostics), mPath(path), mStackPos(0), mDataPos(0), mDeclaredVariables()
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

	char* Emitter::Format(char* fmt, ...)
	{
		char* out = new char[FORMAT__MAX];

		va_list vl;
		va_start(vl, fmt);
		vsprintf_s(out, FORMAT__MAX, fmt, vl);
		va_end(vl);

		return out;
	}

	char* Emitter::Push(char* reg)
	{
		Write("\tpush%c   %s", Mod(), reg);

		return reg;
	}

	char* Emitter::Pop(char* reg)
	{
		Write("\tpop%c    %s", Mod(), reg);

		return reg;
	}

	char* Emitter::Cast(char* reg, int from, int to)
	{
		if (from == to)
			Write("\tmov%c    %s, %s", Mod(to), reg, RegAx(to));
		else if (reg == RegAx(from));
		else
		{
			Write("\tmovs%c%c  %s, %s", Mod(from), Mod(to), reg, RegAx(to));
		}

		return RegAx(to);
	}

	void Emitter::Emit(const CompilationUnitNode* unit)
	{
		for (const MemberNode* member : unit->GetMembers())
			EmitMember(member);
	}

	char* Emitter::EmitMember(const MemberNode* member)
	{
		if (member->Is<FunctionDeclarationNode>())
			return EmitFunctionDeclaration(member->Cast<FunctionDeclarationNode>());
		if (member->Is<GlobalStatementNode>())
			return EmitStatement(member->Cast<GlobalStatementNode>()->GetStatement());

		return nullptr;
	}

	char* Emitter::EmitFunctionDeclaration(const FunctionDeclarationNode* declaration)
	{
		if (declaration->Is<FunctionHintNode>() || declaration->Is<ExternFunctionNode>())
			return nullptr;

		unsigned int pStackPos = mStackPos;
		mStackPos = 0;

		std::string name(declaration->GetName()->GetLex());
		if (name[0] != '$')
			Write(".global _%s", name.c_str());

		Write("_%s:", name.c_str());
		Write("\tpush%c   %s", Mod(), RegBp);
		Write("\tmov%c    %s, %s", Mod(), RegSp, RegBp);

		const std::map<std::string_view, const VariableDeclarationNode*> pDeclaredVariables = mDeclaredVariables;

		unsigned int argumentPos = 4;
		for (const FunctionArgumentNode* argument : declaration->GetArguments()->GetArguments())
		{
			argumentPos += 4;
			Write("_%s$ = %i", std::string(argument->GetName()->GetLex()).c_str(), argumentPos);
			mDeclaredVariables.insert({ argument->GetName()->GetLex(), new VariableDeclarationNode(argument->GetName(), argument->GetType(), nullptr) });
		}

		unsigned int pReturnPos = mReturnPos;
		mReturnPos = mDataPos++;
		for (const StatementNode* statement : declaration->GetBody()->GetStatements())
			EmitStatement(statement);

		Write("..%i:", mReturnPos);
		Write("\tmov%c    %s, %s", Mod(), RegBp, RegSp);
		Write("\tpop%c    %s", Mod(), RegBp);
		Write("\tret%c", Mod());

		mReturnPos = pReturnPos;

		mDeclaredVariables = pDeclaredVariables;
		mStackPos = pStackPos;

		return nullptr;
	}

	char* Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<ReturnStatementNode>())
			return EmitReturnStatement(statement->Cast<ReturnStatementNode>());
		if (statement->Is<ExpressionStatementNode>())
			return EmitExpressionStatement(statement->Cast<ExpressionStatementNode>());
		if (statement->Is<VariableDeclarationNode>())
			return EmitVariableDeclaration(statement->Cast<VariableDeclarationNode>());

		return nullptr;
	}

	char* Emitter::EmitReturnStatement(const ReturnStatementNode* statement)
	{
		Write("\tmov%c    %s, %s", Mod(), EmitExpression(statement->GetExpression()), RegAx());
		Write("\tjmp     ..%i", mReturnPos);

		return nullptr;
	}

	char* Emitter::EmitExpressionStatement(const ExpressionStatementNode* statement)
	{
		Write("\tmov%c    %s, %s", Mod(), EmitExpression(statement->GetExpression()), RegAx());

		return nullptr;
	}

	char* Emitter::EmitVariableDeclaration(const VariableDeclarationNode* declaration)
	{
		std::string name(declaration->GetName()->GetLex());
		unsigned int size = declaration->GetType()->GetSize();
		mStackPos += size;

		mDeclaredVariables.insert({ declaration->GetName()->GetLex(), declaration });

		Write("_%s$ = -%i", name.c_str(), mStackPos);
		Write("\tsub%c    $%i, %s", Mod(), size, RegSp);
		if (declaration->GetInitializer()->GetKind() != Node::Kind::Expression)
		{
			Write("\tmov%c    %s, _%s$(%s)", Mod(size), Cast(EmitExpression(declaration->GetInitializer()), 4, size), name.c_str(), RegBp);
		}

		return Format("_%s$(%s)", name.c_str(), RegBp);
	}

	char* Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->Is<UnaryExpressionNode>())
			return EmitUnaryExpression(expression->Cast<UnaryExpressionNode>());
		if (expression->Is<ParenthesizedExpressionNode>())
			return EmitExpression(expression->Cast<ParenthesizedExpressionNode>()->GetExpression());
		if (expression->Is<BinaryExpressionNode>())
			return EmitBinaryExpression(expression->Cast<BinaryExpressionNode>());
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>());
		if (expression->Is<VariableExpressionNode>())
			return EmitVariableExpression(expression->Cast<VariableExpressionNode>());
		if (expression->Is<FunctionCallExpressionNode>())
			return EmitFunctionCallExpression(expression->Cast<FunctionCallExpressionNode>());
		
		return nullptr;
	}

	char* Emitter::EmitUnaryExpression(const UnaryExpressionNode* expression)
	{
		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Minus:
			Write("\tmov%c    %s, %s", Mod(), EmitExpression(expression->GetValue()), RegAx());
			Write("\tneg%c    %s", Mod(), RegAx());
			return RegAx();
		}

		return nullptr;
	}

	char* Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression)
	{
		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Plus:
			Push(EmitExpression(expression->GetRight()));
			Write("\tmov%c    %s, %s", Mod(), EmitExpression(expression->GetLeft()), RegAx());
			Write("\tadd%c    %s, %s", Mod(), Pop(RegDx()), RegAx());
			return RegAx();
		case Token::Kind::Minus:
			Push(EmitExpression(expression->GetRight()));
			Write("\tmov%c    %s, %s", Mod(), EmitExpression(expression->GetLeft()), RegAx());
			Write("\tsub%c    %s, %s", Mod(), Pop(RegDx()), RegAx());
			return RegAx();
		case Token::Kind::Asterisk:
			Push(EmitExpression(expression->GetRight()));
			Write("\tmov%c    %s, %s", Mod(), EmitExpression(expression->GetLeft()), RegAx());
			Write("\timul%c   %s, %s", Mod(), Pop(RegDx()), RegAx());
			return RegAx();
		case Token::Kind::Slash:
			Write("\tmov%c    %s, %s", Mod(), EmitExpression(expression->GetLeft()), RegAx());
			Write("\tcltd");
			Write("\tmov%c    %s, %s", Mod(), EmitExpression(expression->GetLeft()), RegCx);
			Write("\tidiv%c   %s", Mod(), RegCx);
			return RegAx();
		}

		return nullptr;
	}

	char* Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression)
	{
		if (expression->Is<NumberLiteralExpressionNode>())
			return Format("$%s", std::string(expression->GetLiteral()->GetLex()).c_str());
		if (expression->Is<BooleanLiteralExpressionNode>())
			return Format("$%i", expression->GetLiteral()->Is(Token::Kind::True));
		if (expression->Is<StringLiteralExpressionNode>())
			return EmitStringLiteralExpression(expression->Cast<StringLiteralExpressionNode>());

		return nullptr;
	}

	char* Emitter::EmitVariableExpression(const VariableExpressionNode* expression, bool set)
	{
		std::string name(expression->GetName()->GetLex());
		if (!VariableDefined(name))
		{
			mDiagnostics->ReportError(expression->GetName(), "'%s' is not a Variable", name.c_str());
			return RegErr;
		}

		int size = mDeclaredVariables[name]->GetType()->GetSize();
		if (set)
			return Format("_%s$(%s)", Mod(), name.c_str(), RegBp);

		return Cast(Format("_%s$(%s)", name.c_str(), RegBp), size);
	}

	char* Emitter::EmitFunctionCallExpression(const FunctionCallExpressionNode* call)
	{
		const FunctionDeclarationNode* function = mDiagnostics->GetFunction(call->GetName()->GetLex());
		if (!function)
		{
			mDiagnostics->ReportError(call->GetName(), "Function does not Exist");

			return RegErr;
		}

		unsigned int pushedSize = call->GetArguments()->GetArguments().size();
		for (int i = call->GetArguments()->GetArguments().size() - 1; i >= 0; i--)
		{
			Write("\tpush%c   %s", Mod(), EmitExpression(call->GetArguments()->GetArguments()[i]));
		}

		Write("\tcall%c   _%s", Mod(), std::string(call->GetName()->GetLex()).c_str());
		Write("\tadd%c    $%i, %s", Mod(), pushedSize, RegSp);

		return RegAx();
	}

	char* Emitter::EmitStringLiteralExpression(const StringLiteralExpressionNode* call)
	{
		unsigned int dataPos = mDataPos++;

		WriteLiteral("..%i:", dataPos);
		WriteLiteral("\t.string \"%s\"", std::string(call->GetLiteral()->GetLex()).c_str());

		return Format("$..%i", dataPos);
	}

	char* Emitter::EmitModifiableExpression(const ModifiableExpressionNode* expression)
	{
		
	}

	char* Emitter::EmitAssignmentExpression(const AssignmentExpressionNode* expression)
	{
		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Equal:
			Write("\t");
			return RegAx();
		}
	}

	bool Emitter::VariableDefined(const std::string_view& name)
	{
		return mDeclaredVariables.find(name) != mDeclaredVariables.end();
	}

	#pragma region File Stuff
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
	#pragma endregion
}