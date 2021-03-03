#include "Symple/Code/Ast/ExpressionAst.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;

	AstKind ExpressionAst::GetKind() const
	{ return AstKind::Expression; }


	LiteralExpressionAst::LiteralExpressionAst(GlobalRef<const Token_t> literal)
		: m_Literal(literal) {}

	AstKind LiteralExpressionAst::GetKind() const
	{ return AstKind::LiteralExpression; }

	WeakRef<const Token> LiteralExpressionAst::GetToken() const
	{ return m_Literal; }

	GlobalRef<const Token> LiteralExpressionAst::GetLiteral() const
	{ return m_Literal; }

	void LiteralExpressionAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Literal->Print(os << '\n', indent, "Literal = ");
	}
}