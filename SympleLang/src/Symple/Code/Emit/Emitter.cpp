#include "Symple/Code/Emit/Emitter.h"

namespace Symple::Code
{
	Emitter::Emitter(const GlobalRef<const CompilationUnitAst> &unit)
		: m_Unit(unit), m_File("bin/Out.S", FilePermissions::Write) {}

	void Emitter::Emit()
	{
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
		auto name = fn->Name->Text;

		Emit(".global _%.*s", name.length(), name.data());
		Emit("_%.*s:", name.length(), name.data());
		Emit("\tpush %s", Reg(RegKind::Bp));
		Emit("\tmov %s, %s", Reg(RegKind::Sp), Reg(RegKind::Bp));
		Emit("");

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
		}
	}


	void Emitter::Emit(const GlobalRef<const ExpressionAst> &expr)
	{
		switch (expr->Kind)
		{
		case AstKind::CallExpression:
			Emit(Cast<const CallExpressionAst>(expr));
			break;
		case AstKind::BinaryExpression:
			Emit(Cast<const BinaryExpressionAst>(expr));
			break;
		case AstKind::LiteralExpression:
		{
			auto literal = expr->Token.lock()->Text;
			Emit("\tmov $%.*s, %s", literal.length(), literal.data(), Reg(RegKind::Ax));
			break;
		}

		default:
			Emit("\txor %s, %s # Empty expr", Reg(RegKind::Ax), Reg(RegKind::Ax));
			break;
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
		}
	}


	template<typename... Args>
	void Emitter::Emit(const char *fmt, Args&&... args)
	{
		std::fprintf(m_File.Stream, fmt, args...);
		std::fputc('\n', m_File.Stream);
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