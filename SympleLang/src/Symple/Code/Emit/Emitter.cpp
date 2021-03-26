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
		Emit("\tcall _Sy$Main$Func");
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
		decltype(auto) name = fn->MangledName;

		Emit(".global %s", name.c_str());
		Emit("%s:", name.c_str());
		Emit("\tpush %s", Reg(RegKind::Bp));
		Emit("\tmov %s, %s", Reg(RegKind::Sp), Reg(RegKind::Bp));
		Emit("");

		unsigned int stackPos = 4;
		for (auto param : fn->Parameters)
		{
			stackPos += 4;
			auto pname = param->Name->Text;
			Emit("_%.*s$%u = %u", pname.length(), pname.data(), 2, stackPos);
		}

		Emit(fn->Body);

		Emit("");
		Emit("\tmov %s, %s", Reg(RegKind::Bp), Reg(RegKind::Sp));
		Emit("\tpop %s", Reg(RegKind::Bp));
		Emit("\tret");
	}


	void Emitter::Emit(const GlobalRef<const StatementAst> &stmt)
	{
		switch (stmt->Kind)
		{
		case AstKind::BlockStatement:
			for (auto piece : Cast<const BlockStatementAst>(stmt)->Statements)
				Emit(piece);
			break;
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
		auto name = var->Name->Text;
		uint32 sz = var->Type->Type->Size;
		m_Stack += 4;
		Emit("_%.*s$%u = -%u", name.length(), name.data(), var->Depth, m_Stack);
		Emit("\tsub $%u, %s", sz, Reg(RegKind::Sp));
		if (var->Initializer)
		{
			Emit(var->Initializer);
			Emit("\tmov %s, _%.*s$%u(%s)", Reg(RegKind::Ax, sz), name.length(), name.data(), var->Depth, Reg(RegKind::Bp));
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
			Emit("\tmov %s, -4(%s)", Reg(RegKind::Ax), Reg(RegKind::Sp));
			Emit("\tfildl -4(%s)", Reg(RegKind::Sp));
			Emit("\tfsts -4(%s)", Reg(RegKind::Sp));
			Emit("\tmov -4(%s), %s", Reg(RegKind::Sp), Reg(RegKind::Ax));
		}
		else if (!cast->Type->IsFloat && cast->Value->Type->IsFloat)
		{
			Emit("\tfsts -4(%s)", Reg(RegKind::Sp));
			Emit("\cvttsd2si -4(%s), %s", Reg(RegKind::Sp), Reg(RegKind::Ax));
		}
		else if (cast->Type->Size != cast->Value->Type->Size)
		{
			uint32 min = std::min(cast->Type->Size, cast->Value->Type->Size);
			Emit("\tmovz%c%c %s, %s", Suf(min), Suf(), Reg(RegKind::Ax, min), Reg(RegKind::Ax));
		}
	}

	void Emitter::Emit(const GlobalRef<const CallExpressionAst> &call)
	{
		auto name = call->Name->Text;

		uint32 off = call->Parameters.size() * 4;
		for (auto param : call->Parameters)
		{
			Emit(param);
			Emit("\tmov %s, %u(%s)", Reg(RegKind::Ax), off, Reg(RegKind::Sp));
			off -= 4;
		}
		Emit("\tsub $%i, %s", call->Parameters.size() * 4, Reg(RegKind::Sp));
		Emit("\tcall _%.*s", name.length(), name.data());
		Emit("\tadd $%i, %s", call->Parameters.size() * 4, Reg(RegKind::Sp));
	}

	void Emitter::Emit(const GlobalRef<const NameExpressionAst> &name)
	{
		auto sname = name->Name->Text;
		Emit("\tmov _%.*s$%u(%s), %s", sname.length(), sname.data(), name->Depth, Reg(RegKind::Bp), Reg(RegKind::Ax, name->Type->Size));
	}

	void Emitter::Emit(const GlobalRef<const BinaryExpressionAst> &expr)
	{
		Emit(expr->Right);
		Emit("\tpush %s", Reg(RegKind::Ax));
		Emit(expr->Left);
		Emit("\tpop %s", Reg(RegKind::Bx));

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
		}
	}

	void Emitter::Emit(const GlobalRef<const LiteralExpressionAst> &expr)
	{
		auto literal = expr->Literal->Text;
		Emit("\tmov $%.*s, %s", literal.length(), literal.data(), Reg(RegKind::Ax));
	}


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
			}

		return nullptr;
	}
}