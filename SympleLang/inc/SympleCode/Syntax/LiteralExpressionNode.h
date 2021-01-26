#pragma once

#include "SympleCode/Syntax/ExpressionNode.h"

namespace Symple
{
	class LiteralExpressionNode : public ExpressionNode
	{
	public:
		LiteralExpressionNode(shared_ptr<Token> tok)
			: ExpressionNode(tok) {}

		virtual Kind GetKind()
		{
			return LiteralExpression;
		}

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Literal Expression Node [";
			GetLiteral()->PrintShort(os);
			os << ']';
		}

		shared_ptr<Token> GetLiteral()
		{ return GetToken(); }
	};
}