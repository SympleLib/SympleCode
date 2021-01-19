#include "SympleCode/Emit/Emitter.h"

#include <Windows.h>

#define Emit(fmt, ...) ((void)fprintf(mFile, fmt "\n", __VA_ARGS__))
#define EmitResource(fmt, ...) fprintf_s(mResourceFile, fmt "\n", __VA_ARGS__)

namespace Symple
{
#if SY_32
	const char* const Emitter::sRegisters32[Emitter::NumRegisters] = { "%eax", "%edx", "%ecx", "%ebx", };
	const char* const Emitter::sRegisters16[Emitter::NumRegisters] = { "%ax", "%dx",  "%cx",  "%bx", };
	const char* const Emitter::sRegisters8[Emitter::NumRegisters] = { "%al", "%dl",  "%cl",  "%bl", };
#else
	const char* const Emitter::sRegisters64[Emitter::NumRegisters] = { "%rax", "%rdx", "%rcx", "%rbx", "%rdi", "%rsi",
		"%r8",  "%r9",  "%r10",  "%r11",  "%r12",  "%r13", };
	const char* const Emitter::sRegisters32[Emitter::NumRegisters] = { "%eax", "%edx", "%ecx", "%ebx", "%edi", "%esi",
		"%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", };
	const char* const Emitter::sRegisters16[Emitter::NumRegisters] = { "%ax", "%dx",  "%cx",  "%bx",  "%di",  "%si" ,
		"%r8w", "%r9w", "%r10w", "%r11w", "%r12w", "%r13w", };
	const char* const Emitter::sRegisters8[Emitter::NumRegisters] = { "%al", "%dl",  "%cl",  "%bl",  "%dil", "%sil",
		"%r8b", "%r9b", "%r10b", "%r11b", "%r12b", "%r13b", };
#endif

	unsigned int Emitter::sInits = 0;

	Emitter::Emitter(const char* path)
		: mPath(path), mFile(), mResourceFile(), mData(), mReturn(), mStack(), mReturning()
	{
		while (OpenFile());
		while (OpenResourceFile());
	}

	Emitter::~Emitter()
	{
		rewind(mResourceFile);

		char c;
		while ((c = getc(mResourceFile)) != EOF)
			putc(c, mFile);

		fclose(mResourceFile);
		fclose(mFile);
	}

	void Emitter::EmitCompilationUnit(const CompilationUnitNode* unit)
	{
		for (const MemberNode* member : unit->GetMembers())
			EmitMember(member);
	}

	void Emitter::EmitStaticInitialization()
	{
		Emit(".global  ._Sy@StaticInitialization_.");
		Emit("._Sy@StaticInitialization_.:");

		for (unsigned int i = 0; i < sInits; i++)
			Emit("\tcall%c   ..%i.", Suf(), i);

		Emit("\txor%c    %s, %s", Suf(), GetReg(regax), GetReg(regax));
		Emit("\tret");
	}


	Register Emitter::AllocReg(Register reg)
	{
		if (reg != nullreg && mFreeRegisters[reg])
		{
			mFreeRegisters[reg] = false;
			return reg;
		}

		for (reg = 0; reg < NumRegisters; reg++)
			if (mFreeRegisters[reg])
			{
				mFreeRegisters[reg] = false;
				return reg;
			}

		abort();
	}

	void Emitter::FreeReg(Register reg)
	{
		if (reg == nullreg)
			return;

		if (mFreeRegisters[reg])
			abort();

		mFreeRegisters[reg] = true;
	}

	void Emitter::FreeAllRegs()
	{
		for (int i = 0; i < NumRegisters; i++)
			mFreeRegisters[i] = true;
	}

	const char* Emitter::GetReg(Register reg, int sz)
	{
		if (reg == nullreg)
			return nullptr;

		if (reg == regsp)
		{
			if (sz <= 2)
				return "%sp";
			if (sz <= 4)
				return "%esp";
#if SY_64
			if (sz <= 8)
				return "%rsp";
#endif
		}

		if (reg == regbp)
		{
			if (sz <= 2)
				return "%bp";
			if (sz <= 4)
				return "%ebp";
#if SY_64
			if (sz <= 8)
				return "%rbp";
#endif
		}

		if (sz <= 1)
			return sRegisters8[reg];
		if (sz <= 2)
			return sRegisters16[reg];
		if (sz <= 4)
			return sRegisters32[reg];
#if SY_64
		if (sz <= 8)
			return sRegisters64[reg];
#endif

		return nullptr;
	}


	char Emitter::Suf(int sz)
	{
		if (sz <= 1)
			return 'b';
		if (sz <= 2)
			return 'w';
		if (sz <= 4)
			return 'l';
#if SY_64
		if (sz <= 8)
			return 'q';
#endif

		return 0;
	}

	const char* Emitter::Word(int sz)
	{
		if (sz <= 1)
			return "byte";
		if (sz <= 2)
			return "value";
		if (sz <= 4)
			return "long";
#if SY_64
		if (sz <= 8)
			return "quad";
#endif

		return nullptr;
	}


	void Emitter::Push(Register reg)
	{
		Emit("\tpush    %s", GetReg(reg));
		mStack += platsize;
	}

	void Emitter::Pop(Register reg)
	{
		Emit("\tpop     %s", GetReg(reg));
		mStack -= platsize;
	}


	int Emitter::Align(int n, int align)
	{
		return (n + align - 1) / align * align;
	}

	void Emitter::PushStruct(const StructInitializerExpressionNode* ztruct)
	{
		const TypeNode* ty = ztruct->GetType();

		if (!ty->GetSize())
			return;

		unsigned int sz = Align(ty->GetSize(), platsize);
		Emit("\tsub     $%i, %s", sz, GetReg(regsp));
		mStack += sz;

		for (unsigned int i = 0, off = 0; i < ztruct->GetExpressions().size(); i++)
		{
			Register reg = EmitExpression(ztruct->GetExpressions()[i]);
			Emit("\tmov     %s, %i(%s)", GetReg(reg), off, GetReg(regsp));
			FreeReg(reg);

			off += ztruct->GetExpressions()[i]->GetType()->GetSize();
		}
	}

	void Emitter::PushStruct(const StructDeclarationNode* ty, Register ptr)
	{
		if (!ty->GetSize())
			return;
		
		unsigned int sz = Align(ty->GetSize(), platsize);
		Emit("\tsub     $%i, %s", sz, GetReg(regsp));
		mStack += sz;

		Register freg = AllocReg();
		for (unsigned int i = 0; i < sz; i += platsize)
		{
			if (i)
				Emit("\tmov     %i(%s), %s", i, GetReg(ptr), GetReg(freg));
			else
				Emit("\tmov     (%s), %s", GetReg(ptr), GetReg(freg));

			Emit("\tmov     %s, %i(%s)", GetReg(freg), i, GetReg(regsp));
		}
		FreeReg(freg);
	}

	void Emitter::MovStruct(const StructDeclarationNode* ty, Register from, Register to)
	{
		if (!ty->GetSize())
			return;

		unsigned int sz = Align(ty->GetSize(), platsize);

		Register freg = AllocReg();
		for (unsigned int i = 0; i < sz; i += platsize)
		{
			if (i)
				Emit("\tmov     %i(%s), %s", i, GetReg(from), GetReg(freg));
			else
				Emit("\tmov     (%s), %s", GetReg(from), GetReg(freg));

			Emit("\tmov     %s, %i(%s)", GetReg(freg), i, GetReg(to));
		}
		FreeReg(freg);
	}

	void Emitter::MovStruct(const StructInitializerExpressionNode* ztruct, Register ptr)
	{
		Register reg = AllocReg();

		const TypeNode* ty = ztruct->GetType();
		unsigned int sz = Align(ty->GetSize(), platsize);

		for (unsigned int i = 0, off = 0; i < ztruct->GetExpressions().size(); i++)
		{
			Register reg = EmitExpression(ztruct->GetExpressions()[i]);
			Emit("\tmov     %s, %i(%s)", GetReg(reg), off, GetReg(ptr));
			FreeReg(reg);

			off += ztruct->GetExpressions()[i]->GetType()->GetSize();
		}
	}


	void Emitter::Loc(const Token* tok)
	{
		Emit(".loc 1 %i %i", tok->GetLine(), tok->GetColumn());
	}


	void Emitter::EmitMember(const MemberNode* member)
	{
		FreeAllRegs(); // In Case I forgot to Free a Register

		switch (member->GetKind())
		{
		case Node::Kind::GlobalStatement:
			return EmitGlobalStatement(member->Cast<GlobalStatementNode>());
		case Node::Kind::FunctionDeclaration:
			return EmitFunctionDeclaration(member->Cast<FunctionDeclarationNode>());
		}
	}

	void Emitter::EmitGlobalStatement(const GlobalStatementNode* member)
	{
		EmitStatement(member->GetStatement());
	}

	void Emitter::EmitFunctionDeclaration(const FunctionDeclarationNode* member)
	{
		mStack ^= mStack; // Reset stack

		const char* name = member->GetAsmName().c_str();

		if (!member->GetModifiers()->IsPrivate())
			Emit("\t.global   %s", name);
		Emit("%s:", name);

		Debug::BeginScope();

		unsigned int depth = Debug::GetDepth();
		unsigned int off = platsize;
		for (const FunctionArgumentNode* argument : member->GetArguments()->GetArguments())
		{
			off += platsize;
			Debug::VariableDeclaration(argument);
			Emit("_%s$%i = %i", std::string(argument->GetName()->GetLex()).c_str(), depth, off);
		}

		Push(regbp);
		Emit("\tmov     %s, %s", GetReg(regsp), GetReg(regbp));

		if (member->IsMain())
			Emit("\tcall    ._Sy@StaticInitialization_.");


		mReturning = false;
		for (const StatementNode* statement : member->GetBody()->GetStatements())
			if (statement->IsReturn() == 1 && statement != member->GetBody()->GetStatements().back())
			{
				mReturning = true;
				break;
			}

		if (mReturning)
			mReturn = mData++;

		if (member->GetBody()->GetStackUsage())
			Emit("\tsub     $%i, %s", member->GetBody()->GetStackUsage(), GetReg(regsp));

		for (const StatementNode* statement : member->GetBody()->GetStatements())
		{
			EmitStatement(statement);
			if (statement->IsReturn() == 2)
				break;
		}

		Debug::EndScope();

		if (mReturning)
			Emit("..%i:", mReturn);

		Emit("\tmov     %s, %s", GetReg(regbp), GetReg(regsp));
		Pop(regbp);
		Emit("\tret");
	}


	void Emitter::EmitStatement(const StatementNode* statement)
	{
		FreeAllRegs(); // In Case I forgot to Free a Register

		switch (statement->GetKind())
		{
		case Node::Kind::ExpressionStatement:
			return EmitExpressionStatement(statement->Cast<ExpressionStatementNode>());
		case Node::Kind::VariableDeclaration:
			return EmitVariableDeclaration(statement->Cast<VariableDeclarationNode>());
		}
	}

	void Emitter::EmitExpressionStatement(const ExpressionStatementNode* statement)
	{
		Register reg = EmitExpression(statement->GetExpression());
		if (reg != regax)
			Emit("\tmov     %s, %s", GetReg(reg), GetReg(regax));

		FreeAllRegs(); // In Case I forgot to Free a Register
	}

	void Emitter::EmitVariableDeclaration(const VariableDeclarationNode* statement)
	{
		Debug::VariableDeclaration(statement);

		std::string nameStr(statement->GetName()->GetLex());
		const char* name = nameStr.c_str();
		unsigned int depth = Debug::GetDepth();
		unsigned int sz = statement->GetType()->GetSize();

		Emit("_%s$%i = -%i", name, depth, mStack += sz);

		if (statement->GetInitializer())
		{
			if (statement->GetInitializer()->CanEvaluate())
			{
				Emit("\tmov%c    $%i, _%s$%i(%s)", Suf(sz), statement->GetInitializer()->Evaluate(), name, depth, GetReg(regbp));
			}
			else
			{
				if (statement->GetInitializer()->GetType()->GetType()->Is<StructDeclarationNode>() && !statement->GetInitializer()->GetType()->GetContinue())
				{
					Register left = AllocReg();
					Emit("\tlea     _%s$%i(%s), %s", name, depth, GetReg(regbp), GetReg(left));

					if (statement->GetInitializer()->Is<StructInitializerExpressionNode>())
					{
						MovStruct(statement->GetInitializer()->Cast<StructInitializerExpressionNode>(), left);
					}
					else
					{
						Register right = EmitModifiableExpression(statement->GetInitializer()->Cast<ModifiableExpressionNode>(), true);
						MovStruct(statement->GetInitializer()->GetType()->GetType()->Cast<StructDeclarationNode>(), right, left);
					}

					FreeReg(left);
				}
				else
				{
					Register init = EmitExpression(statement->GetInitializer());
					Emit("\tmov     %s, _%s$%i(%s)", GetReg(init), name, depth, GetReg(regbp));
					FreeReg(init);
				}
			}
		}

		if (statement->GetNext())
			EmitVariableDeclaration(statement->GetNext());
	}


	Register Emitter::EmitExpression(const ExpressionNode* expression)
	{
		if (expression->CanEvaluate())
		{
			Register reg = AllocReg();
			if (expression->Evaluate())
				Emit("\tmov     $%i, %s", expression->Evaluate(), GetReg(reg));
			else
				Emit("\txor     %s, %s", GetReg(reg), GetReg(reg));
			return reg;
		}

		if (expression->Is<ModifiableExpressionNode>())
			return EmitModifiableExpression(expression->Cast<ModifiableExpressionNode>());

		switch (expression->GetKind())
		{
		case Node::Kind::CastExpression:
			return EmitCastExpression(expression->Cast<CastExpressionNode>());
		case Node::Kind::ListExpression:
			return EmitListExpression(expression->Cast<ListExpressionNode>());
		case Node::Kind::UnaryExpression:
			return EmitUnaryExpression(expression->Cast<UnaryExpressionNode>());
		case Node::Kind::BinaryExpression:
			return EmitBinaryExpression(expression->Cast<BinaryExpressionNode>());
		case Node::Kind::StallocExpression:
			return EmitStallocExpression(expression->Cast<StallocExpressionNode>());
		case Node::Kind::FunctionCallExpression:
			return EmitFunctionCallExpression(expression->Cast<FunctionCallExpressionNode>());
		case Node::Kind::ParenthesizedExpression:
			return EmitParenthesizedExpression(expression->Cast<ParenthesizedExpressionNode>());
		case Node::Kind::StringLiteralExpression:
			return EmitStringLiteralExpression(expression->Cast<StringLiteralExpressionNode>());
		case Node::Kind::VariableAddressExpression:
			return EmitVariableAddressExpression(expression->Cast<VariableAddressExpressionNode>());
		}

		return nullreg;
	}

	Register Emitter::EmitCastExpression(const CastExpressionNode* expression)
	{
		return EmitExpression(expression->GetExpression());
	}

	Register Emitter::EmitListExpression(const ListExpressionNode* expression)
	{
		unsigned int sz = expression->GetExpressionType()->GetSize();

		Emit("\tsub     $%i, %s", expression->GetExpressions().size() * sz, GetReg(regsp));
		mStack += expression->GetExpressions().size() * sz;
		unsigned int off = 0;
		for (unsigned int i = 0; i < expression->GetExpressions().size(); i++)
		{
			const ExpressionNode* item = expression->GetExpressions()[i];

			if (item->CanEvaluate())
				Emit("\tmov%c    $%i, -%i(%s)", Suf(sz), item->Evaluate(), mStack - off, GetReg(regbp));
			else
			{
				Register ireg = EmitExpression(item);
				Emit("\tmov%c    %s, -%i(%s)", Suf(sz), GetReg(ireg), mStack - off, GetReg(regbp));
				FreeReg(ireg);
			}

			off += sz;
		}

		Register reg = AllocReg();
		Emit("\tlea     -%i(%s), %s", mStack, GetReg(regbp), GetReg(reg));
		return reg;
	}

	Register Emitter::EmitStallocExpression(const StallocExpressionNode* expression)
	{
		Register reg;

		if (expression->GetSize()->CanEvaluate())
		{
			reg = AllocReg();
			Emit("\tsub     $%i, %s", expression->GetSize()->Evaluate(), GetReg(regsp));
		}
		else
		{
			reg = EmitExpression(expression->GetSize());
			Emit("\tsub     %s, %s", GetReg(reg), GetReg(regsp));
		}

		Emit("\tmov     %s, %s", GetReg(regsp), GetReg(reg));
		return reg;
	}

	Register Emitter::EmitFunctionCallExpression(const FunctionCallExpressionNode* expression)
	{
		bool axUsed = !mFreeRegisters[regax];
		if (axUsed)
			Push(regax);

		for (unsigned int i = expression->GetArguments()->GetArguments().size(); i > 0; i--)
		{
			switch (expression->GetArguments()->GetArguments()[i - 1]->GetKind())
			{
			case Node::Kind::StructInitializerExpression:
				PushStruct(expression->GetArguments()->GetArguments()[i - 1]->Cast<StructInitializerExpressionNode>());
				break;
			default:
				if (expression->GetArguments()->GetArguments()[i - 1]->GetType()->GetType()->Is<StructDeclarationNode>() && !expression->GetArguments()->GetArguments()[i - 1]->GetType()->GetContinue())
				{
					Register reg = EmitModifiableExpression(expression->GetArguments()->GetArguments()[i - 1]->Cast<ModifiableExpressionNode>(), true);
					PushStruct(expression->GetArguments()->GetArguments()[i - 1]->GetType()->GetType()->Cast<StructDeclarationNode>(), reg);
					FreeReg(reg);
				}
				else
				{
					Register reg = EmitExpression(expression->GetArguments()->GetArguments()[i - 1]);
					Push(reg);
					FreeReg(reg);
				}
				break;
			}
		}

		Emit("\tcall    %s", Debug::GetFunction(expression->GetName()->GetLex(), expression->GetArguments())->GetAsmName().c_str());
		Emit("\tadd     $%i, %s", expression->GetArguments()->GetArguments().size() * platsize, GetReg(regsp));
		mStack -= expression->GetArguments()->GetArguments().size() * platsize;
		Register reg = AllocReg(regax);
		if (reg != regax)
			Emit("\tmov     %s, %s", GetReg(regax), GetReg(reg));

		if (axUsed)
			Pop(regax);

		return reg;
	}

	Register Emitter::EmitParenthesizedExpression(const ParenthesizedExpressionNode* expression)
	{
		return EmitExpression(expression->GetExpression());
	}

	Register Emitter::EmitVariableAddressExpression(const VariableAddressExpressionNode* expression)
	{
		return EmitModifiableExpression(expression->GetVariable(), true);
	}


	Register Emitter::EmitModifiableExpression(const ModifiableExpressionNode* expression, bool retptr)
	{
		switch (expression->GetKind())
		{
		case Node::Kind::FieldExpression:
			return EmitFieldExpression(expression->Cast<FieldExpressionNode>(), retptr);
		case Node::Kind::VariableExpression:
			return EmitVariableExpression(expression->Cast<VariableExpressionNode>(), retptr);
		case Node::Kind::AssignmentExpression:
			return EmitAssignmentExpression(expression->Cast<AssignmentExpressionNode>(), retptr);
		case Node::Kind::PointerIndexExpression:
			return EmitPointerIndexExpression(expression->Cast<PointerIndexExpressionNode>(), retptr);
		case Node::Kind::DereferencePointerExpression:
			return EmitDereferencePointerExpression(expression->Cast<DereferencePointerExpressionNode>(), retptr);
		}

		return nullreg;
	}

	Register Emitter::EmitFieldExpression(const FieldExpressionNode* expression, bool retptr)
	{
		Register reg;
		unsigned int off = expression->GetOffset();

		if (off)
		{
			reg = EmitModifiableExpression(expression->GetCallee(), true);

			if (retptr)
				Emit("\tadd     $%i, %s", off, GetReg(reg));
			else
				Emit("\tmov     %i(%s), %s", off, GetReg(reg), GetReg(reg));
		}
		else if (retptr)
			reg = EmitModifiableExpression(expression->GetCallee(), true);
		else
			reg = EmitModifiableExpression(expression->GetCallee(), false);

		return reg;
	}

	Register Emitter::EmitVariableExpression(const VariableExpressionNode* expression, bool retptr)
	{
		const VariableDeclaration* decl = Debug::GetVariable(expression->GetName()->GetLex());

		std::string nameStr(decl->GetName()->GetLex());
		const char* name = nameStr.c_str();
		unsigned int depth = Debug::GetVariableDepth(expression->GetName()->GetLex());
		unsigned int sz = decl->GetType()->GetSize();
		sz = sz > platsize ? platsize : sz;

		Register reg = AllocReg();
		if (retptr)
			Emit("\tlea     _%s$%i(%s), %s", name, depth, GetReg(regbp), GetReg(reg));
		else
			Emit("\tmov     _%s$%i(%s), %s", name, depth, GetReg(regbp), GetReg(reg, sz));

		return reg;
	}

	Register Emitter::EmitAssignmentExpression(const AssignmentExpressionNode* expression, bool retptr)
	{
		unsigned int sz = expression->GetType()->GetSize();
		Register left = EmitModifiableExpression(expression->GetLeft(), true);

		if (expression->GetRight()->GetType()->GetType()->Is<StructDeclarationNode>() && !expression->GetRight()->GetType()->GetContinue())
		{
			if (expression->GetRight()->Is<StructInitializerExpressionNode>())
			{
				MovStruct(expression->GetRight()->Cast<StructInitializerExpressionNode>(), left);
			}
			else
			{
				Register right = EmitModifiableExpression(expression->GetRight()->Cast<ModifiableExpressionNode>(), true);
				MovStruct(expression->GetRight()->GetType()->GetType()->Cast<StructDeclarationNode>(), right, left);
			}
		}
		else if (expression->GetRight()->CanEvaluate())
		{
			switch (expression->GetOperator()->GetKind())
			{
			case Token::Kind::Equal:
				Emit("\tmov%c    $%i, (%s)", Suf(sz), expression->GetRight()->Evaluate(), GetReg(left));
				goto ReturnEval;
			case Token::Kind::PlusEqual:
				Emit("\tadd%c    $%i, (%s)", Suf(sz), expression->GetRight()->Evaluate(), GetReg(left));
				goto ReturnEval;
			case Token::Kind::MinusEqual:
				Emit("\tsub%c    $%i, (%s)", Suf(sz), expression->GetRight()->Evaluate(), GetReg(left));
				goto ReturnEval;
			case Token::Kind::AsteriskEqual:
				Register out = AllocReg();
				Emit("\timul%c   $%i, (%s), %s", Suf(sz), expression->GetRight()->Evaluate(), GetReg(left), GetReg(out, sz));
				Emit("\tmov     %s, (%s)", GetReg(out, sz), GetReg(left));
				FreeReg(out);
				goto ReturnEval;
			}

			return nullreg;

		ReturnEval:
			if (retptr)
				return left;
			else
			{
				Register right = AllocReg();

				if (sz != platsize)
					Emit("\txor     %s, %s", GetReg(right), GetReg(right));
				Emit("\tmov     (%s), %s", GetReg(left), GetReg(right, sz));
				FreeReg(left);

				return right;
			}
		}
		else
		{
			Register right = EmitExpression(expression->GetRight());

			switch (expression->GetOperator()->GetKind())
			{
			case Token::Kind::Equal:
				Emit("\tmov     %s, (%s)", GetReg(right, sz), GetReg(left));
				goto Return;
			case Token::Kind::PlusEqual:
				Emit("\tadd     %s, (%s)", GetReg(right, sz), GetReg(left));
				goto Return;
			case Token::Kind::MinusEqual:
				Emit("\tsub     %s, (%s)", GetReg(right, sz), GetReg(left));
				goto Return;
			case Token::Kind::AsteriskEqual:
				Emit("\timul%c   (%s), %s", Suf(sz), GetReg(left), GetReg(right, sz));
				Emit("\tmov     %s, (%s)", GetReg(right, sz), GetReg(left));
				goto Return;
			}

			return nullreg;

		Return:
			if (retptr)
			{
				FreeReg(right);
				return left;
			}
			else
			{
				if (expression->GetType()->GetSize() != platsize)
					Emit("\txor     %s, %s", GetReg(right), GetReg(right));
				Emit("\tmov     (%s), %s", GetReg(left), GetReg(right, expression->GetType()->GetSize()));
				FreeReg(left);

				return right;
			}
		}

		return nullreg;
	}

	Register Emitter::EmitPointerIndexExpression(const PointerIndexExpressionNode* expression, bool retptr)
	{
		unsigned int sz = expression->GetType()->GetSize();
		Register addr = EmitExpression(expression->GetAddress());

		if (expression->GetIndex()->CanEvaluate())
		{
			if (expression->GetIndex()->Evaluate())
				if (retptr)
					Emit("\tadd     $%i, %s", expression->GetIndex()->Evaluate() * sz, GetReg(addr));
				else
					Emit("\tmov     %i(%s), %s", expression->GetIndex()->Evaluate() * sz, GetReg(addr), GetReg(addr));
			else
				if (!retptr)
					Emit("\tmov     (%s), %s", GetReg(addr), GetReg(addr));
		}
		else
		{
			Register idx = EmitExpression(expression->GetIndex());
			if (sz != 1)
				Emit("\timul    $%i, %s", sz, GetReg(idx));
			Emit("\tadd     %s, %s", GetReg(idx), GetReg(addr));
			FreeReg(idx);

			if (!retptr)
				Emit("\tmov     (%s), %s", GetReg(addr), GetReg(addr));
		}

		return addr;
	}

	Register Emitter::EmitDereferencePointerExpression(const DereferencePointerExpressionNode* expression, bool retptr)
	{
		Register reg = EmitExpression(expression->GetAddress());
		if (retptr)
			return reg;
		else
		{
			Emit("\tmov     (%s), %s", GetReg(reg), GetReg(reg));
			return reg;
		}
	}


	Register Emitter::EmitUnaryExpression(const UnaryExpressionNode* expression)
	{
		Register reg = EmitExpression(expression->GetValue());

		switch (expression->GetOperator()->GetKind())
		{
		case Token::Kind::Exclamation:
			Emit("\ttest    %s, %s", GetReg(reg), GetReg(reg));
			Emit("\tsete    %s", GetReg(reg, 1));
			Emit("\tmovzb%c  %s, %s", Suf(), GetReg(reg, 1), GetReg(reg));
			return reg;
		case Token::Kind::Minus:
			Emit("\tneg     %s", GetReg(reg));
			return reg;
		}

		return nullreg;
	}

	Register Emitter::EmitBinaryExpression(const BinaryExpressionNode* expression)
	{
		if (expression->GetRight()->CanEvaluate())
		{
			Register left = EmitExpression(expression->GetLeft());
			
			switch (expression->GetOperator()->GetKind())
			{
			case Token::Kind::Plus:
				Emit("\tadd     $%i, %s", expression->GetRight()->Evaluate(), GetReg(left));
				return left;
			case Token::Kind::Minus:
				Emit("\tsub     $%i, %s", expression->GetRight()->Evaluate(), GetReg(left));
				return left;
			case Token::Kind::Asterisk:
				Emit("\timul    $%i, %s, %s", expression->GetRight()->Evaluate(), GetReg(left), GetReg(left));
				return left;
			}
		}
		else
		{
			Register left = EmitExpression(expression->GetLeft());
			Register right = EmitExpression(expression->GetRight());
			FreeReg(right);
			
			switch (expression->GetOperator()->GetKind())
			{
			case Token::Kind::Plus:
				Emit("\tadd     %s, %s", GetReg(right), GetReg(left));
				return left;
			case Token::Kind::Minus:
				Emit("\tsub     %s, %s", GetReg(right), GetReg(left));
				return left;
			case Token::Kind::Asterisk:
				Emit("\timul    %s, %s", GetReg(right), GetReg(left));
				return left;
			}
		}

		return nullreg;
	}


	Register Emitter::EmitStringLiteralExpression(const StringLiteralExpressionNode* literal)
	{
		EmitResource("..%i:", mData);
		EmitResource("\t.string \"%s\"", literal->Stringify().c_str());

		Register reg = AllocReg();
		Emit("\tmov     $..%i, %s", mData++, GetReg(reg));
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

	bool Emitter::OpenResourceFile()
	{
		errno_t err = tmpfile_s(&mResourceFile);
		if (err && !mResourceFile)
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
