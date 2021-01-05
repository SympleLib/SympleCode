#pragma once

#include "SympleCode/Node/Expression/Literal/LiteralExpressionNode.h"

namespace Symple
{
	class NullLiteralExpressionNode : public LiteralExpressionNode
	{
	public:
		NullLiteralExpressionNode(const Token* literal)
			: LiteralExpressionNode(PtrType, literal) {}

		Kind GetKind() const override
		{
			return Kind::NullLiteralExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Null Literal Expression";

			return ss.str();
		}
	};
}