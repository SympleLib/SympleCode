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
#include "SympleCode/Common/Node/Expression/CharacterLiteralExpressionNode.h"

#define Write(fmt, ...) ((void)fprintf_s(mFile, fmt "\n", __VA_ARGS__))
#define WriteLiteral(fmt, ...) ((void)(mWriting ? fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__) : 0))

#define RegErr "ERROR"

#define RegCx "%ecx"
#define RegSp "%esp"
#define RegBp "%ebp"

#define FORMAT__MAX 128

namespace Symple
{
	Emitter::Emitter(Diagnostics* diagnostics, const char* path)
		: mDiagnostics(diagnostics), mPath(path), mStackPos(0), mDataPos(0), mDeclaredVariables(), mWriting(true)
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

	char Emitter::Rep(int size)
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

	void Emitter::EnableWrite()
	{
		mWriting = true;
	}

	void Emitter::DisableWrite()
	{
		mWriting = false;
	}

	char* Emitter::Push(char* reg, int size)
	{
		Write("\tpush%c   %s", Rep(size), reg);

		return reg;
	}

	char* Emitter::Pop(char* reg, int size)
	{
		Write("\tpop%c    %s", Rep(size), reg);

		return reg;
	}

	char* Emitter::Move(char* from, char* to, int size)
	{
		if (strcmp(from, to))
			Write("\tmov%c    %s, %s", Rep(size), from, to);

		return to;
	}

	char* Emitter::Cast(char* rfrom, char* rto, int from, int to)
	{
		if (!strcmp(rfrom, rto));
		else if (from == to)
			Move(rfrom, rto, to);
		else if (rfrom[0] == '$' && rfrom[1] != '.')
			Move(rfrom, rto, to);
		else if (!strcmp(rfrom, RegAx(from)))
			Move(RegAx(to), rto, to);
		else if (!strcmp(rfrom, RegDx(from)))
			Move(RegDx(to), rto, to);
		else
			Write("\tmovs%c%c  %s, %s", Rep(from), Rep(to), rfrom, rto);

		return rto;
	}

	char* Emitter::Lea(char* from, char* to)
	{
		Write("\tlea%c    %s, %s", Rep(), from, to);
		return to;
	}

	char* Emitter::Add(char* right, char* left)
	{
		Write("\tadd%c    %s, %s", Rep(), right, left);
		return left;
	}

	char* Emitter::Sub(char* right, char* left)
	{
		Write("\tsub%c    %s, %s", Rep(), right, left);
		return left;
	}

	char* Emitter::Mul(char* right, char* left)
	{
		Write("\timul%c   %s, %s", Rep(), right, left);
		return left;
	}

	char* Emitter::Div(char* left)
	{
		if (strcmp(left, RegCx))
			Move(left, RegCx);
		if (strcmp(left, RegAx()))
			Move(RegCx, RegAx());
		Write("\tcltd");
		Pop(RegCx);
		Write("\tidiv%c   %s", Rep(), RegCx);
		return left;
	}

	char* Emitter::Mod(char* left)
	{
		Div(left);
		if (strcmp(left, RegDx()))
			Move(RegDx(), left);
		return left;
	}

	char* Emitter::Xor(char* right, char* left, int size)
	{
		Write("\txor%c    %s, %s", Rep(size), right, left);
		return left;
	}

	char* Emitter::Neg(char* val)
	{
		Write("\tneg%c    %s", Rep(), val);
		return val;
	}

	char* Emitter::Cmp(char* right, char* left, int size)
	{
		Write("\tcmp%c    %s, %s", Rep(size), right, left);
		return left;
	}

	char* Emitter::CmpOp(const Token* oqerator)
	{
		switch (oqerator->GetKind())
		{
		case Token::Kind::EqualEqual:
			return "e";
		case Token::Kind::ExclamationEqual:
			return "ne";
		case Token::Kind::LeftArrow:
			return "l";
		case Token::Kind::LeftArrowEqual:
			return "le";
		case Token::Kind::RightArrow:
			return "g";
		case Token::Kind::RightArrowEqual:
			return "ge";
		}

		return nullptr;
	}

	char* Emitter::CmpNOp(const Token* oqerator)
	{
		switch (oqerator->GetKind())
		{
		case Token::Kind::EqualEqual:
			return "ne";
		case Token::Kind::ExclamationEqual:
			return "e";
		case Token::Kind::LeftArrow:
			return "ge";
		case Token::Kind::RightArrow:
			return "le";
		}

		return nullptr;
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
		Write("\tpush%c   %s", Rep(), RegBp);
		Move(RegSp, RegBp);

		const std::map<std::string_view, const VariableDeclarationNode*> pDeclaredVariables = mDeclaredVariables;

		unsigned int argumentPos = 4;
		for (const FunctionArgumentNode* argument : declaration->GetArguments()->GetArguments())
		{
			argumentPos += 4;
			Write("_%s$ = %i", std::string(argument->GetName()->GetLex()).c_str(), argumentPos);
			mDeclaredVariables.insert({ argument->GetName()->GetLex(), argument });
		}

		unsigned int pReturnPos = mReturnPos;
		mReturnPos = mDataPos++;
		for (const StatementNode* statement : declaration->GetBody()->GetStatements())
			EmitStatement(statement);

		Write("..%i:", mReturnPos);
		Move(RegBp, RegSp);
		Write("\tpop%c    %s", Rep(), RegBp);
		Write("\tret%c", Rep());

		mReturnPos = pReturnPos;

		mDeclaredVariables = pDeclaredVariables;
		mStackPos = pStackPos;

		return nullptr;
	}

	char* Emitter::EmitStatement(const StatementNode* statement)
	{
		Write("");
		if (statement->Is<BlockStatementNode>())
			return EmitBlockStatement(statement->Cast<BlockStatementNode>());
		if (statement->Is<IfStatementNode>())
			return EmitIfStatement(statement->Cast<IfStatementNode>());
		if (statement->Is<BreakStatementNode>())
			return EmitBreakStatement(statement->Cast<BreakStatementNode>());
		if (statement->Is<ReturnStatementNode>())
			return EmitReturnStatement(statement->Cast<ReturnStatementNode>());
		if (statement->Is<ExpressionStatementNode>())
			return EmitExpressionStatement(statement->Cast<ExpressionStatementNode>());
		if (statement->Is<VariableDeclarationNode>())
			return EmitVariableDeclaration(statement->Cast<VariableDeclarationNode>());

		return nullptr;
	}

	char* Emitter::EmitBlockStatement(const BlockStatementNode* body)
	{
		const std::map<std::string_view, const VariableDeclarationNode*> pDeclaredVariables = mDeclaredVariables;

		for (const StatementNode* statement : body->GetStatements())
			EmitStatement(statement);

		mDeclaredVariables = pDeclaredVariables;

		return nullptr;
	}

	char* Emitter::EmitIfStatement(const IfStatementNode* declaration)
	{
		bool elze = declaration->GetElse();
		unsigned int elsePos = elze ? mDataPos++ : mDataPos;
		unsigned int endPos = mDataPos++;

		Cmp("$0", EmitExpression(declaration->GetCondition()));
		Write("\tje      ..%i", elsePos);

		EmitBlockStatement(declaration->GetThen());

		if (elze)
		{
			Write("\tjmp     ..%i", endPos);
			Write("..%i:", elsePos);

			EmitBlockStatement(declaration->GetElse());
		}

		Write("..%i:", endPos);

		return nullptr;
	}

	char* Emitter::EmitBreakStatement(const BreakStatementNode* statement)
	{
		Write("\tjmp     ..%i", mBreakPoints.back());
		mBreakPoints.pop_back();

		return nullptr;
	}

	char* Emitter::EmitWhileStatement(const WhileStatementNode* declaration)
	{
		unsigned int loopPos = mDataPos++, endPos = mDataPos++;

		Write("..%i:", loopPos);

		Cmp("$1", EmitExpression(declaration->GetCondition()));
		Write("\tje      ..%i", endPos);

		mBreakPoints.push_back(endPos);

		EmitBlockStatement(declaration->GetBody());

		Write("\tjmp     ..%i", loopPos);
		Write("..%i:", endPos);

		return nullptr;
	}

	char* Emitter::EmitReturnStatement(const ReturnStatementNode* statement)
	{
		Move(EmitExpression(statement->GetExpression()), RegAx());
		Write("\tjmp     ..%i", mReturnPos);

		return nullptr;
	}

	char* Emitter::EmitExpressionStatement(const ExpressionStatementNode* statement)
	{
		Move(EmitExpression(statement->GetExpression()), RegAx());

		return nullptr;
	}

	char* Emitter::EmitVariableDeclaration(const VariableDeclarationNode* declaration)
	{
		std::string name(declaration->GetName()->GetLex());
		unsigned int size = declaration->GetType()->GetSize();
		mStackPos += size;

		mDeclaredVariables.insert({ declaration->GetName()->GetLex(), declaration });
		
		Write("_%s$ = -%i", name.c_str(), mStackPos);
		Write("\tsub%c    $%i, %s", Rep(), size, RegSp);
		if (declaration->GetInitializer()->GetKind() != Node::Kind::Expression)
			Move(Cast(EmitExpression(declaration->GetInitializer()), RegAx(), 4, size), Format("_%s$(%s)", name.c_str(), RegBp), size);

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
		if (expression->Is<PointerIndexExpressionNode>())
			return EmitPointerIndexExpression(expression->Cast<PointerIndexExpressionNode>());
		if (expression->Is<AssignmentExpressionNode>())
			return EmitAssignmentExpression(expression->Cast<AssignmentExpressionNode>());
		
		return nullptr;
	}

	char* Emitter::EmitUnaryExpression(const UnaryExpressionNode* expression)
	{
		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Exclamation:
			Move(EmitExpression(expression->GetValue()), RegAx());
			return Xor("$1");
		case Token::Kind::Minus:
			Move(EmitExpression(expression->GetValue()), RegAx());
			return Neg(RegAx());
		}

		return nullptr;
	}

	char* Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression)
	{
		Push(EmitExpression(expression->GetRight()));
		Move(EmitExpression(expression->GetLeft()), RegAx());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Plus:
			Add(Pop(RegDx()), RegAx());
			return RegAx();
		case Token::Kind::Minus:
			Sub(Pop(RegDx()), RegAx());
			return RegAx();
		case Token::Kind::Asterisk:
			Mul(Pop(RegDx()), RegAx());
			return RegAx();
		case Token::Kind::Slash:
			Div(RegAx());
			return RegAx();
		case Token::Kind::Percentage:
			Mod(RegAx());
			return RegAx();

		case Token::Kind::EqualEqual:
		case Token::Kind::LeftArrow:
		case Token::Kind::RightArrow:
		case Token::Kind::LeftArrowEqual:
		case Token::Kind::RightArrowEqual:
			Cmp(Pop(RegDx()), RegAx());
			Write("\tset%s    %s", CmpOp(expression->GetOperator()), RegAx(1));
			return Cast(RegAx(1), RegAx(), 1);
		case Token::Kind::ExclamationEqual:
			Cmp(Pop(RegDx()), RegAx());
			Write("\tset%s   %s", CmpOp(expression->GetOperator()), RegAx(1));
			return Cast(RegAx(1), RegAx(), 1);
		}

		return nullptr;
	}

	char* Emitter::EmitLiteralExpression(const LiteralExpressionNode* expression)
	{
		if (expression->Is<NumberLiteralExpressionNode>())
			return Format("$%s", std::string(expression->GetLiteral()->GetLex()).c_str());
		if (expression->Is<BooleanLiteralExpressionNode>())
			return Format("$%i", expression->GetLiteral()->Is(Token::Kind::True));
		if (expression->Is<CharacterLiteralExpressionNode>())
			return Format("$%i", expression->GetLiteral()->GetLex()[0]);
		if (expression->Is<StringLiteralExpressionNode>())
			return EmitStringLiteralExpression(expression->Cast<StringLiteralExpressionNode>());

		return nullptr;
	}

	char* Emitter::EmitVariableExpression(const VariableExpressionNode* expression)
	{
		std::string name(expression->GetName()->GetLex());
		if (!VariableDefined(name))
		{
			mDiagnostics->ReportError(expression->GetName(), "'%s' is not a Variable", name.c_str());
			return RegErr;
		}

		int size = mDeclaredVariables[name]->GetType()->GetSize();

		return Cast(Format("_%s$(%s)", name.c_str(), RegBp), RegAx(), size);
	}

	char* Emitter::EmitFunctionCallExpression(const FunctionCallExpressionNode* call)
	{
		const FunctionDeclarationNode* function = mDiagnostics->GetFunction(call->GetName()->GetLex());
		if (!function)
		{
			mDiagnostics->ReportError(call->GetName(), "Function does not Exist");

			return RegErr;
		}

		unsigned int pushedSize = call->GetArguments()->GetArguments().size() * 4;
		for (int i = call->GetArguments()->GetArguments().size() - 1; i >= 0; i--)
			Write("\tpush%c   %s", Rep(), EmitExpression(call->GetArguments()->GetArguments()[i]));

		Write("\tcall%c   _%s", Rep(), std::string(call->GetName()->GetLex()).c_str());
		Write("\tadd%c    $%i, %s", Rep(), pushedSize, RegSp);

		return RegAx();
	}

	char* Emitter::EmitPointerIndexExpression(const PointerIndexExpressionNode* expression)
	{
		ModifiableExpression modifiableExpression = EmitModifiablePointerIndexExpression(expression);
		return Cast(modifiableExpression.Emit, RegAx(), modifiableExpression.Size);
	}

	char* Emitter::EmitStringLiteralExpression(const StringLiteralExpressionNode* expression)
	{
		unsigned int dataPos = mDataPos++;

		WriteLiteral("..%i:", dataPos);
		WriteLiteral("\t.string \"%s\"", std::string(expression->GetLiteral()->GetLex()).c_str());

		return Format("$..%i", dataPos);
	}

	char* Emitter::EmitAssignmentExpression(const AssignmentExpressionNode* expression)
	{
		ModifiableExpression modifiableExpression = EmitModifiableAssignmentExpression(expression);
		return Cast(modifiableExpression.Emit, RegAx(), modifiableExpression.Size);
	}

	Emitter::ModifiableExpression Emitter::EmitModifiableExpression(const ModifiableExpressionNode* expression)
	{
		if (expression->Is<VariableExpressionNode>())
			return EmitModifiableVariableExpression(expression->Cast<VariableExpressionNode>());
		if (expression->Is<AssignmentExpressionNode>())
			return EmitModifiableAssignmentExpression(expression->Cast<AssignmentExpressionNode>());
		if (expression->Is<PointerIndexExpressionNode>())
			return EmitModifiablePointerIndexExpression(expression->Cast<PointerIndexExpressionNode>());

		return { nullptr, 0 };
	}

	Emitter::ModifiableExpression Emitter::EmitModifiableVariableExpression(const VariableExpressionNode* expression)
	{
		std::string name(expression->GetName()->GetLex());
		if (!VariableDefined(name))
		{
			mDiagnostics->ReportError(expression->GetName(), "'%s' is not a Variable", name.c_str());
			return { RegErr, 0 };
		}

		int size = mDeclaredVariables[name]->GetType()->GetSize();
		return { Format("_%s$(%s)", name.c_str(), RegBp), size };
	}

	Emitter::ModifiableExpression Emitter::EmitModifiableAssignmentExpression(const AssignmentExpressionNode* expression)
	{
		DisableWrite();
		ModifiableExpression modifiableExpression = EmitModifiableExpression(expression->GetLeft());
		int size = modifiableExpression.Size;
		EnableWrite();
		Push(EmitExpression(expression->GetRight()));
		EmitModifiableExpression(expression->GetLeft());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Equal:
			Cast(Pop(RegDx()), modifiableExpression.Emit, 4, size);
			return modifiableExpression;
		case Token::Kind::PlusEqual:
			Cast(Add(Pop(RegCx), Cast(modifiableExpression.Emit, RegDx(), size)), modifiableExpression.Emit, 4, size);
			return modifiableExpression;
		case Token::Kind::MinusEqual:
			Cast(Sub(Pop(RegCx), Cast(modifiableExpression.Emit, RegDx(), size)), modifiableExpression.Emit, 4, size);
			return modifiableExpression;
		case Token::Kind::SlashEqual:
			Cast(Div(Cast(modifiableExpression.Emit, RegDx(), size)), modifiableExpression.Emit, 4, size);
			return modifiableExpression;
		case Token::Kind::PercentageEqual:
			Cast(Mod(Cast(modifiableExpression.Emit, RegDx(), size)), modifiableExpression.Emit, 4, size);
			return modifiableExpression;
		}

		return { nullptr, 0 };
	}

	Emitter::ModifiableExpression Emitter::EmitModifiablePointerIndexExpression(const PointerIndexExpressionNode* expression)
	{
		ModifiableExpression modifiableExpression = EmitModifiableExpression(expression->GetExpression());
		Move(modifiableExpression.Emit);
		Add(EmitExpression(expression->GetIndex()));

		return { Format("(%s)", RegAx()), 1 };
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