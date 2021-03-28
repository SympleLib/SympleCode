#include "Symple/Code/Emit/Emitter.h"

namespace Symple::Code
{
	Emitter::Emitter(const GlobalRef<const CompilationUnitAst> &unit)
		: m_Unit(unit), m_File("bin/Out.S", FilePermissions::Write) {}

	void Emitter::Emit()
	{
		Emit(".global _main");
		Emit("_main:");
		Emit("\txor %s, %s", Reg(RegKind::Ax), Reg(RegKind::Ax));
		Emit("\tpush %u(%s)", 8, Reg(RegKind::Sp));
		Emit("\tpush %u(%s)", 8, Reg(RegKind::Sp));
		Emit("\tcall _Sy$Main$Func");
		Emit("\tadd $%u, %s", 8, Reg(RegKind::Sp));
		Emit("\tmovss %s, -%u(%s)", Reg(RegKind::Xmm0), 4, Reg(RegKind::Sp));
		Emit("\tmov -%u(%s), %s", 4, Reg(RegKind::Sp), Reg(RegKind::Ax));
		Emit("\tret");

		for (auto member : m_Unit->Members)
			switch (member->Kind)
			{
			case AstKind::Function:
				Emit(Cast<const FunctionAst>(member));
				break;
			}

		m_File.Close();
	}


	void Emitter::Emit(const GlobalRef<const FunctionAst> &fn)
	{
		m_Stack = m_StackSize = 0;
		decltype(auto) name = fn->MangledName;

		Emit(".global %s", name.c_str());
		Emit("%s:", name.c_str());
		Emit("\tpush %s", Reg(RegKind::Bp));
		Emit("\tmov %s, %s", Reg(RegKind::Sp), Reg(RegKind::Bp));
		Emit("\tsub $%s.StackSize, %s", name.c_str(), Reg(RegKind::Sp));
		Emit("");

		unsigned int stackPos = 4;
		for (auto param : fn->Parameters)
		{
			stackPos += 4;
			auto pname = param->Name->Text;
			Emit("_%.*s$%u = %u", pname.length(), pname.data(), 1, stackPos);
		}

		Emit(fn->Body);

		Emit("");
		Emit("\tmov %s, %s", Reg(RegKind::Bp), Reg(RegKind::Sp));
		Emit("\tpop %s", Reg(RegKind::Bp));
		Emit("\tret");

		Emit("%s.StackSize = %u", name.c_str(), m_StackSize);
	}


	void Emitter::Emit(const GlobalRef<const StatementAst> &stmt)
	{
		Emit("");

		switch (stmt->Kind)
		{
		case AstKind::BlockStatement:
		{
			uint32 pStack = m_Stack;
			for (auto piece : Cast<const BlockStatementAst>(stmt)->Statements)
				Emit(piece);
			m_Stack = pStack;
			break;
		}
		case AstKind::ReturnStatement:
			Emit(Cast<const ReturnStatementAst>(stmt)->Value);
			break;
		case AstKind::ExpressionStatement:
			Emit(Cast<const ExpressionStatementAst>(stmt)->Expression);
			break;
		case AstKind::VariableStatement:
			Emit(Cast<const VariableStatementAst>(stmt));
			break;
		}
	}

	void Emitter::Emit(const GlobalRef<const VariableStatementAst> &var)
	{
		decltype(auto) name = var->MangledName;
		uint32 sz = var->Type->Type->Size;
		Stalloc(sz);
		uint32 pos = m_Stack;
		Emit("%s = -%u", name.c_str(), m_Stack);
		if (var->Initializer)
		{
			Emit(var->Initializer);
			if (var->Initializer->Type->IsFloat)
				Emit("\tmovss %s, %s(%s)", Reg(RegKind::Xmm0, sz), name.c_str(), Reg(RegKind::Bp));
			else
				Emit("\tmov %s, %s(%s)", Reg(RegKind::Ax, sz), name.c_str(), Reg(RegKind::Bp));
		}
	}


	void Emitter::Emit(const GlobalRef<const ExpressionAst> &expr)
	{
		switch (expr->Kind)
		{
		case AstKind::CastExpression:
			Emit(Cast<const CastExpressionAst>(expr));
			break;
		case AstKind::CallExpression:
			Emit(Cast<const CallExpressionAst>(expr));
			break;
		case AstKind::NameExpression:
			Emit(Cast<const NameExpressionAst>(expr));
			break;
		case AstKind::BinaryExpression:
			Emit(Cast<const BinaryExpressionAst>(expr));
			break;
		case AstKind::LiteralExpression:
			Emit(Cast<const LiteralExpressionAst>(expr));
			break;
		case AstKind::ParenthasizedExpression:
			Emit(Cast<const ParenthasizedExpressionAst>(expr)->Expression);
			break;

		default:
			Emit("\txor %s, %s # Empty expr", Reg(RegKind::Ax), Reg(RegKind::Ax));
			break;
		}
	}

	void Emitter::Emit(const GlobalRef<const CastExpressionAst> &cast)
	{
		Emit(cast->Value);

		if (cast->Type->Is(cast->Value->Type));
		else if (cast->Type->Is(TypeKind::Bool))
		{
			Emit("\ttest %s, %s", Reg(RegKind::Ax), Reg(RegKind::Ax));
			Emit("\tsetne %s", Reg(RegKind::Ax, 1));
			Emit("\tmovz%c%c %s, %s", Suf(1), Suf(), Reg(RegKind::Ax, 1), Reg(RegKind::Ax));
		}
		else if (cast->Type->IsFloat && !cast->Value->Type->IsFloat)
		{
			Stalloc();
			uint32 pos = m_Stack;
			Emit("\tmov %s, -%u(%s)", Reg(RegKind::Ax), pos, Reg(RegKind::Bp));
			Emit("\tcvtsi2ssl -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Xmm0));
			Staf();
		}
		else if (!cast->Type->IsFloat && cast->Value->Type->IsFloat)
		{
			Stalloc();
			uint32 pos = m_Stack;
			Emit("\tmovss %s, -%u(%s)", Reg(RegKind::Xmm0), pos, Reg(RegKind::Bp));
			Emit("\tcvttss2si -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Ax));
			Staf();
		}
		else if (cast->Type->Size != cast->Value->Type->Size)
		{
			uint32 min = std::min(cast->Type->Size, cast->Value->Type->Size);
			Emit("\tmovz%c%c %s, %s", Suf(min), Suf(), Reg(RegKind::Ax, min), Reg(RegKind::Ax));
		}
	}

	void Emitter::Emit(const GlobalRef<const CallExpressionAst> &call)
	{
		uint32 sz = call->Parameters.size() * 4 + 4;
		Emit(call->Function);
		Stalloc(sz);
		Emit("mov %s, -%u(%s)", Reg(RegKind::Ax), m_Stack, Reg(RegKind::Bp));

		uint32 off = call->Parameters.size() * 4;
		for (auto param : call->Parameters)
		{
			Emit(param);
			Emit("\tmov %s, %u(%s)", Reg(RegKind::Ax), off, Reg(RegKind::Sp));
			off -= 4;
		}
		Emit("mov -%u(%s), %s", m_Stack, Reg(RegKind::Bp), Reg(RegKind::Ax));
		Emit("\tcall *%s", Reg(RegKind::Ax));
		Staf(sz);
	}

	void Emitter::Emit(const GlobalRef<const NameExpressionAst> &name)
	{
		decltype(auto) sname = name->Symbol->MangledName;
		if (name->Symbol->IsFunction)
			Emit("\tlea %s, %s", sname.c_str(), Reg(RegKind::Ax, name->Type->Size));
		else
			if (name->Type->IsFloat)
				Emit("\tmovss %s(%s), %s", sname.c_str(), Reg(RegKind::Bp), Reg(RegKind::Xmm0));
			else
				Emit("\tmov %s(%s), %s", sname.c_str(), Reg(RegKind::Bp), Reg(RegKind::Ax, name->Type->Size));
	}

	void Emitter::Emit(const GlobalRef<const BinaryExpressionAst> &expr)
	{
		if (expr->Type->IsFloat)
		{
			Emit(expr->Right);
			Stalloc();
			uint32 pos = m_Stack;
			Emit("\tmovss %s, -%u(%s)", Reg(RegKind::Xmm0), pos, Reg(RegKind::Bp));
			Emit(expr->Left);
			Emit("\tmovss -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Xmm1));
			Staf();

			switch (expr->Operator->Kind)
			{
			case TokenKind::Plus:
				Emit("\taddss %s, %s", Reg(RegKind::Xmm1), Reg(RegKind::Xmm0));
				break;
			case TokenKind::Minus:
				Emit("\tsubss %s, %s", Reg(RegKind::Xmm1), Reg(RegKind::Xmm0));
				break;
			case TokenKind::Star:
				Emit("\tmulss %s, %s", Reg(RegKind::Xmm1), Reg(RegKind::Xmm0));
				break;
			case TokenKind::Slash:
				Emit("\tdivss %s, %s", Reg(RegKind::Xmm1), Reg(RegKind::Xmm0));
				break;
			case TokenKind::Percent:
			{
				Stalloc(8);
				uint32 pos = m_Stack;
				Emit("\tmovss %s, %u(%s)", Reg(RegKind::Xmm0), 0, Reg(RegKind::Sp));
				Emit("\tmovss %s, %u(%s)", Reg(RegKind::Xmm1), 4, Reg(RegKind::Sp));
				Emit("\tcall _fmodf");
				Emit("\tfstps -%u(%s)", pos, Reg(RegKind::Bp));
				Emit("\tmovss -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Xmm0));
				Staf(8);
				break;
			}
			case TokenKind::CarotCarot:
			{
				Stalloc(8);
				uint32 pos = m_Stack;
				Emit("\tmovss %s, %u(%s)", Reg(RegKind::Xmm0), 0, Reg(RegKind::Sp));
				Emit("\tmovss %s, %u(%s)", Reg(RegKind::Xmm1), 4, Reg(RegKind::Sp));
				Emit("\tcall _powf");
				Emit("\tfstps -%u(%s)", pos, Reg(RegKind::Bp));
				Emit("\tmovss -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Xmm0));
				Staf(8);
				break;
			}
			}
		}
		else
		{
			Emit(expr->Right);
			uint32 pos = m_Stack;
			Stalloc();
			Emit("\tmov %s, -%u(%s)", Reg(RegKind::Ax), pos, Reg(RegKind::Bp));
			Emit(expr->Left);
			Emit("\tmov -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Bx));
			Staf();

			switch (expr->Operator->Kind)
			{
			case TokenKind::Plus:
				Emit("\tadd %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;
			case TokenKind::Minus:
				Emit("\tsub %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;
			case TokenKind::Star:
				Emit("\timul %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;
			case TokenKind::Slash:
				Emit("\tmov %s, %s", Reg(RegKind::Bx), Reg(RegKind::Cx));
				Emit("\tcltd");
				Emit("\tidiv %s", Reg(RegKind::Cx));
				break;
			case TokenKind::Percent:
				Emit("\tmov %s, %s", Reg(RegKind::Bx), Reg(RegKind::Cx));
				Emit("\tcltd");
				Emit("\tidiv %s", Reg(RegKind::Cx));
				Emit("\tmov %s, %s", Reg(RegKind::Dx), Reg(RegKind::Ax));
				break;
			case TokenKind::Carot:
				Emit("\txor %s, %s", Reg(RegKind::Bx), Reg(RegKind::Ax));
				break;
			}
		}
	}

	void Emitter::Emit(const GlobalRef<const LiteralExpressionAst> &expr)
	{
		auto literal = expr->Literal->Text;

		if (expr->Type->IsFloat)
		{
			union
			{
				float fVal;
				int iVal;
			};
			fVal = strtod(literal.data(), nullptr);
			Stalloc();
			uint32 pos = m_Stack;
			Emit("\tmovl $0x%x, -%u(%s) # Float %f", iVal, pos, Reg(RegKind::Bp), fVal);
			Emit("\tmovss -%u(%s), %s", pos, Reg(RegKind::Bp), Reg(RegKind::Xmm0));
			Staf();
		}
		else
			Emit("\tmov $%.*s, %s", literal.length(), literal.data(), Reg(RegKind::Ax));
	}


	void Emitter::Stalloc(uint32 bytes)
	{
		m_Stack += bytes;
		if (m_Stack > m_StackSize)
			m_StackSize = m_Stack;
	}

	void Emitter::Staf(uint32 bytes)
	{ m_Stack -= bytes; }


	template<typename... Args>
	void Emitter::Emit(_Printf_format_string_ const char *fmt, Args&&... args)
	{
		std::fprintf(m_File.Stream, fmt, args...);
		std::fputc('\n', m_File.Stream);
	}

	constexpr const char Emitter::Suf(uint32 sz)
	{
		if (sz <= 1)
			return 'b';
		if (sz <= 2)
			return 'w';
		if (sz <= 4)
			return 'l';
	}

	constexpr const char *Emitter::Reg(RegKind kind, uint32 sz)
	{
		if (sz)
			switch (kind)
			{
			case RegKind::Ax:
				if (sz <= 1)
					return "%al";
				if (sz <= 2)
					return "%ax";
				if (sz <= 4)
					return "%eax";
				break;
			case RegKind::Bx:
				if (sz <= 1)
					return "%bl";
				if (sz <= 2)
					return "%bx";
				if (sz <= 4)
					return "%ebx";
				break;
			case RegKind::Cx:
				if (sz <= 1)
					return "%cl";
				if (sz <= 2)
					return "%cx";
				if (sz <= 4)
					return "%ecx";
				break;
			case RegKind::Dx:
				if (sz <= 1)
					return "%dl";
				if (sz <= 2)
					return "%dx";
				if (sz <= 4)
					return "%edx";
				break;

			case RegKind::Sp:
				return "%esp";
			case RegKind::Bp:
				return "%ebp";

			case RegKind::Xmm0:
				return "%xmm0";
			case RegKind::Xmm1:
				return "%xmm1";
			case RegKind::Xmm2:
				return "%xmm2";
			case RegKind::Xmm3:
				return "%xmm3";
			case RegKind::Xmm4:
				return "%xmm4";
			case RegKind::Xmm5:
				return "%xmm5";
			case RegKind::Xmm6:
				return "%xmm6";
			case RegKind::Xmm7:
				return "%xmm7";
			}

		return nullptr;
	}
}