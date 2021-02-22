#pragma once

#include "Symple/Syntax/ExpressionSyntax.h"

namespace Symple
{
	class SY_API LiteralExpressionSyntax: public ExpressionSyntax
	{
	private:
		const GlobalRef<Token> literal;
	public:
		LiteralExpressionSyntax(const GlobalRef<Token> &literal)
			: literal(literal) {}

		virtual SyntaxKind GetKind() const override
		{ return SyntaxKind::LiteralExpression; }

		virtual GlobalRef<Token> GetToken() const override
		{ return literal; }

		GlobalRef<Token> GetLiteral() const
		{ return literal; }

		SY_PROPERTY_GET(GetLiteral) GlobalRef<Token> Literal;
	};
}