#pragma once

#include "SympleCode/Common/Node/Expression/LiteralExpressionNode.h"

namespace Symple
{
	class NumberLiteralExpressionNode : public LiteralExpressionNode
	{
	public:
		NumberLiteralExpressionNode(const Token* literal)
			: LiteralExpressionNode(literal) {}

		Kind GetKind() const override
		{
			return Kind::NumberLiteralExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Number Literal Expression (" << mLiteral->GetLex() << ")";

			return ss.str();
		}
	};
}