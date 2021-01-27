#pragma once

#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class LiteralExpressionSyntax : public ExpressionSyntax
	{
	public:
		LiteralExpressionSyntax(shared_ptr<Token> tok)
			: ExpressionSyntax(tok) {}

		virtual Kind GetKind()
		{ return LiteralExpression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Literal Expression Syntax [";
			GetLiteral()->PrintShort(os);
			os << ']';
		}

		shared_ptr<Token> GetLiteral()
		{ return GetToken(); }
	};
}