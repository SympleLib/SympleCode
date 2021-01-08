#include "SympleCode/Emit/Emitter.h"

#include <Windows.h>

#define Emit(fmt, ...) fprintf(mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__)

#define FORMAT_MAX 16

namespace Symple
{
	static Emit RegAx = { nullptr, "%eax" };
	static Emit RegDx = { nullptr, "%edx" };
	static Emit RegCx = { nullptr, "%ecx" };
	static Emit RegBx = { nullptr, "%ebx" };

	static Emit RegSi = { nullptr, "%esi" };
	static Emit RegDi = { nullptr, "%edi" };

	static Emit RegBp = { nullptr, "%ebp" };
	static Emit RegSp = { nullptr, "%esp" };

	Emitter::Emitter(const char* path)
		: mPath(path), mFile(), mLiteralFile(), mData(), mReturn(), mStack(),
			mReturning()
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


	Emit Emitter::Lea(Emit from, Emit to)
	{
		Emit("\tleal    %s, %s", from.Eval, to.Eval);

		return { to };
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
			//if (from.Node && !from.Node->Evaluate())
			//	Emit("\txorl    %s, %s", to.Eval, to.Eval);
			//else
				Emit("\tmovl    %s, %s", from.Eval, to.Eval);

		return { nullptr, to.Eval };
	}


	Emit Emitter::Add(Emit from, Emit to)
	{
		Emit("\taddl    %s, %s", from.Eval, to.Eval);

		return { nullptr, to.Eval };
	}

	Emit Emitter::Sub(Emit from, Emit to)
	{
		Emit("\tsubl    %s, %s", from.Eval, to.Eval);

		return { nullptr, to.Eval };
	}

	Emit Emitter::Xor(Emit from, Emit to)
	{
		Emit("\txorl    %s, %s", from.Eval, to.Eval);

		return { nullptr, to.Eval };
	}


	Emit Emitter::Neg(Emit emit)
	{
		Emit("\tnegl    %s", emit.Eval);

		return { nullptr, emit.Eval };
	}


	Emit Emitter::EmitMember(const MemberNode* member)
	{
		if (member->GetKind() == Node::Kind::FunctionDeclaration)
			return EmitFunctionDeclaration(member->Cast<FunctionDeclarationNode>());

		return {};
	}

	Emit Emitter::EmitFunctionDeclaration(const FunctionDeclarationNode* declaration)
	{
		mStack = 0;

		std::string nameStr = declaration->GetAsmName();
		const char* name = nameStr.c_str();

		if (!declaration->GetModifiers()->IsPrivate())
			Emit(".global %s", name);

		Debug::BeginScope();

		for (unsigned int i = 0; i < declaration->GetArguments()->GetArguments().size(); i++)
			Emit("_%s$ = %i", std::string(declaration->GetArguments()->GetArguments()[i]->GetName()->GetLex()).c_str(),
				i * 4 + 4);

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

		Debug::EndScope();

		if (mReturning)
			Emit("..%i:", mReturn);
		Move(RegBp, RegSp);
		Pop(RegBp);
		Emit("\tret");

		return { nullptr, name };
	}

	Emit Emitter::EmitStatement(const StatementNode* statement)
	{
		if (statement->Is<ReturnStatementNode>())
			return EmitReturnStatement(statement->Cast<ReturnStatementNode>());
		if (statement->Is<ExpressionStatementNode>())
			return EmitExpressionStatement(statement->Cast<ExpressionStatementNode>());
		if (statement->Is<VariableDeclarationNode>())
			return EmitVariableDeclaration(statement->Cast<VariableDeclarationNode>());

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
		return Move(EmitExpression(statement->GetExpression()), RegAx);
	}

	Emit Emitter::EmitVariableDeclaration(const VariableDeclarationNode* declaration)
	{
		Debug::VariableDeclaration(declaration);

		std::string nameStr(declaration->GetName()->GetLex());
		const char* name = nameStr.c_str();
		mStack += declaration->GetType()->GetSize();

		Emit("_%s$ = -%i", name, mStack);
		Sub({ nullptr, Format("$%i", declaration->GetType()->GetSize()) }, RegSp);
		if (declaration->GetInitializer())
			Move(EmitExpression(declaration->GetInitializer()), { nullptr, Format("_%s$(%%ebp)", name) });

		if (declaration->GetNext())
			return EmitVariableDeclaration(declaration->GetNext());

		return {};
	}

	Emit Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->Is<CastExpressionNode>())
			return EmitCastExpression(expression->Cast<CastExpressionNode>());
		if (expression->Is<LiteralExpressionNode>())
			return EmitLiteralExpression(expression->Cast<LiteralExpressionNode>());
		if (expression->Is<OperatorExpressionNode>())
			return EmitOperatorExpression(expression->Cast<OperatorExpressionNode>());
		if (expression->Is<ModifiableExpressionNode>())
			return EmitModifiableExpression(expression->Cast<ModifiableExpressionNode>());
		if (expression->Is<FunctionCallExpressionNode>())
			return EmitFunctionCallExpression(expression->Cast<FunctionCallExpressionNode>());
		if (expression->Is<ListExpressionNode>())
			return EmitListExpression(expression->Cast<ListExpressionNode>());

		return { expression };
	}

	Emit Emitter::EmitCastExpression(const CastExpressionNode* expression)
	{
		return EmitExpression(expression->GetExpression());
	}

	Emit Emitter::EmitListExpression(const ListExpressionNode* expression)
	{
		unsigned int size = expression->GetExpression().empty() ? 0 : expression->GetExpression().size() * expression->GetExpression()[0]->GetType()->GetSize();
		Sub({ nullptr, Format("$%i", size) }, RegSp);

		mStack += size;
		for (const ExpressionNode* item : expression->GetExpression())
		{
			Move(EmitExpression(item), { nullptr, Format("-%i(%%ebp)", mStack) });
			mStack -= item->GetType()->GetSize();
		}
		mStack += size;

		Lea({ nullptr, Format("-%i(%%ebp)", mStack) }, RegAx);
		return { expression, RegAx.Eval };
	}

	Emit Emitter::EmitFunctionCallExpression(const FunctionCallExpressionNode* call)
	{
		const FunctionDeclarationNode* function = Debug::GetFunction(call->GetName()->GetLex(), call->GetArguments());
		if (!function)
		{
			Diagnostics::ReportError(call->GetName(), "Function Doesn't Exist");

			return { call };
		}
		
		for (int i = call->GetArguments()->GetArguments().size() - 1; i >= 0; i--)
			Push(EmitExpression(call->GetArguments()->GetArguments()[i]));

		Emit("\tcalll   %s", function->GetAsmName().c_str());
		Add({ nullptr, Format("$%i", call->GetArguments()->GetArguments().size() * 4) }, RegSp );

		return { call, RegAx.Eval };
	}

	Emit Emitter::EmitParenthesizedExpression(const ParenthesizedExpressionNode* expression)
	{
		return { expression, EmitExpression(expression->GetExpression()).Eval };
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

	Emit Emitter::EmitModifiableExpression(const ModifiableExpressionNode* expression)
	{
		if (expression->Is<FieldExpressionNode>())
			return EmitFieldExpression(expression->Cast<FieldExpressionNode>());
		if (expression->Is<VariableExpressionNode>())
			return EmitVariableExpression(expression->Cast<VariableExpressionNode>());
		if (expression->Is<AssignmentExpressionNode>())
			return EmitAssignmentExpression(expression->Cast<AssignmentExpressionNode>());
		if (expression->Is<DereferencePointerExpressionNode>())
			return EmitDereferencePointerExpression(expression->Cast<DereferencePointerExpressionNode>());

		return { expression };
	}

	Emit Emitter::EmitFieldExpression(const FieldExpressionNode* expression)
	{
		const StructDeclarationNode* ztruct = expression->GetCallee()->GetType()->GetType()->Cast<StructDeclarationNode>();

		unsigned int offset = 0;
		for (const VariableDeclarationNode* field : ztruct->GetFields()->GetFields())
		{
			if (field->GetName()->GetLex() == expression->GetName()->GetLex())
				break;
			offset += field->GetType()->GetSize();
		}

		Lea(EmitExpression(expression->GetCallee()), RegAx);
		if (offset)
			Add({ nullptr, Format("$%i", offset) }, RegAx);

		return { expression, "(%eax)" };
	}

	Emit Emitter::EmitVariableExpression(const VariableExpressionNode* expression)
	{
		const VariableDeclaration* variable = Debug::GetVariable(expression->GetName()->GetLex());

		return { expression, Format("_%s$(%%ebp)", std::string(expression->GetName()->GetLex()).c_str()), variable->GetType()->GetSize() };
	}

	Emit Emitter::EmitAssignmentExpression(const AssignmentExpressionNode* expression)
	{
		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Equal:
			return { expression, Move(EmitExpression(expression->GetRight()), EmitExpression(expression->GetLeft())).Eval };
		}

		return { expression };
	}

	Emit Emitter::EmitDereferencePointerExpression(const DereferencePointerExpressionNode* expression)
	{
		Move(EmitExpression(expression->GetAddress()), RegAx);

		return { expression, "(%eax)" };
	}

	Emit Emitter::EmitOperatorExpression(const OperatorExpressionNode* expression)
	{
		if (expression->Is<UnaryExpressionNode>())
			return EmitUnaryExpression(expression->Cast<UnaryExpressionNode>());
		if (expression->Is<BinaryExpressionNode>())
			return EmitBinaryExpression(expression->Cast<BinaryExpressionNode>());

		return { expression };
	}

	Emit Emitter::EmitUnaryExpression(const UnaryExpressionNode* expression)
	{
		Emit value = EmitExpression(expression->GetValue());
		Move(value, RegAx);

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Exclamation:
			if (!(value.Size - 1))
				return { expression, Xor({ nullptr, "$1" }, RegAx).Eval };

			Emit("\ttestl   %%eax, %%eax");
			Emit("\tsete    %%al");
			return { expression, RegAx.Eval, 1 };
		case Token::Kind::Minus:
			return { expression, Neg(RegAx).Eval };
		}

		return { expression };
	}

	Emit Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression)
	{
		Push(EmitExpression(expression->GetRight()));
		Move(EmitExpression(expression->GetLeft()), RegAx);
		Pop(RegDx);

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Plus:
			return { expression, Add(RegDx, RegAx).Eval };
		case Token::Kind::Minus:
			return { expression, Sub(RegDx, RegAx).Eval };
		}

		return { expression };
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