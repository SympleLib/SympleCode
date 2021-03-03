#include "Symple/Code/AST/ExpressionAST.h"

namespace Symple::Code
{
	using Token_t = AST::Token_t;

	ASTKind ExpressionAST::GetKind() const
	{ return ASTKind::Expression; }


	LiteralExpressionAST::LiteralExpressionAST(GlobalRef<const Token_t> literal)
		: m_Literal(literal) {}

	ASTKind LiteralExpressionAST::GetKind() const
	{ return ASTKind::LiteralExpression; }

	WeakRef<const Token> LiteralExpressionAST::GetToken() const
	{ return m_Literal; }

	GlobalRef<const Token> LiteralExpressionAST::GetLiteral() const
	{ return m_Literal; }
}