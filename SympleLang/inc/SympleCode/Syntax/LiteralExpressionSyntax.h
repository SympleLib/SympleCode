#pragma once

#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class LiteralExpressionSyntax : public ExpressionSyntax
	{
	public:
		LiteralExpressionSyntax(shared_ptr<Token> tok)
			: ExpressionSyntax(tok) {}

		virtual Kind GetKind() override
		{ return LiteralExpression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{ GetLiteral()->Print(os, indent, last, label); os << " (Literal Syntax)"; }

		shared_ptr<Token> GetLiteral()
		{ return GetToken(); }
	};
}