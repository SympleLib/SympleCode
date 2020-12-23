#pragma once

#include "SympleCode/Common/Node/Expression/LiteralExpressionNode.h"

namespace Symple
{
	class CharacterLiteralExpressionNode : public LiteralExpressionNode
	{
	public:
		CharacterLiteralExpressionNode(const Token* literal)
			: LiteralExpressionNode(literal) {}

		Kind GetKind() const override
		{
			return Kind::CharacterLiteralExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Character Literal Expression (" << mLiteral->GetLex() << ")";

			return ss.str();
		}
	};
}