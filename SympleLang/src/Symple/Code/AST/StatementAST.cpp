#include "Symple/Code/AST/StatementAST.h"

namespace Symple::Code
{
	using Token_t = AST::Token_t;

	ASTKind StatementAST::GetKind() const
	{ return ASTKind::Statement; }


	ExpressionStatementAST::ExpressionStatementAST(GlobalRef<ExpressionAST> expr)
		: m_Expr(expr) {}

	ASTKind ExpressionStatementAST::GetKind() const
	{ return ASTKind::ExpressionStatement; }

	WeakRef<const Token_t> ExpressionStatementAST::GetToken() const
	{ return m_Expr->Token; }

	GlobalRef<const ExpressionAST> ExpressionStatementAST::GetExpression() const
	{ return m_Expr; }

	void ExpressionStatementAST::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Expr->Print(os << '\n', indent, "Expression = ");
	}


	ReturnStatementAST::ReturnStatementAST(WeakRef<const Token_t> keyword, GlobalRef<ExpressionAST> val)
		: m_Keyword(keyword), m_Value(val) {}

	ASTKind ReturnStatementAST::GetKind() const
	{ return ASTKind::ReturnStatement; }

	WeakRef<const Token_t> ReturnStatementAST::GetToken() const
	{ return m_Keyword; }

	GlobalRef<const ExpressionAST> ReturnStatementAST::GetValue() const
	{ return m_Value; }

	void ReturnStatementAST::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Value->Print(os << '\n', indent, "Value = ");
	}
}