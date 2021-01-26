#pragma once

#include "SympleCode/Syntax/ExpressionNode.h"

namespace Symple
{
	class LiteralExpressionNode : public ExpressionNode
	{
	public:
		LiteralExpressionNode(std::shared_ptr<Token> tok)
			: ExpressionNode(tok) {}

		virtual Kind GetKind()
		{
			return LiteralExpression;
		}

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true)
		{
			PrintIndent(os, indent, last);
			os << "Literal Expression Node [";
			GetLiteral()->PrintShort(os);
			os << ']';
		}

		std::shared_ptr<Token> GetLiteral()
		{ return mToken; }
	};
}