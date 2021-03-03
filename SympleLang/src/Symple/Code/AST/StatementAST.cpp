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
}