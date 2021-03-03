#include "Symple/Code/Ast/StatementAst.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;

	AstKind StatementAst::GetKind() const
	{ return AstKind::Statement; }


	ExpressionStatementAst::ExpressionStatementAst(GlobalRef<ExpressionAst> expr)
		: m_Expr(expr) {}

	AstKind ExpressionStatementAst::GetKind() const
	{ return AstKind::ExpressionStatement; }

	WeakRef<const Token_t> ExpressionStatementAst::GetToken() const
	{ return m_Expr->Token; }

	GlobalRef<const ExpressionAst> ExpressionStatementAst::GetExpression() const
	{ return m_Expr; }

	void ExpressionStatementAst::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Expr->Print(os << '\n', indent, "Expression = ");
	}


	ReturnStatementAst::ReturnStatementAst(WeakRef<const Token_t> keyword, GlobalRef<ExpressionAst> val)
		: m_Keyword(keyword), m_Value(val) {}

	AstKind ReturnStatementAst::GetKind() const
	{ return AstKind::ReturnStatement; }

	WeakRef<const Token_t> ReturnStatementAst::GetToken() const
	{ return m_Keyword; }

	GlobalRef<const ExpressionAst> ReturnStatementAst::GetValue() const
	{ return m_Value; }

	void ReturnStatementAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Value->Print(os << '\n', indent, "Value = ");
	}
}