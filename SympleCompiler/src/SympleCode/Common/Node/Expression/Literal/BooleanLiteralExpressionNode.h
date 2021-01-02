#pragma once

#include "SympleCode/Common/Node/Expression/Literal/LiteralExpressionNode.h"

namespace Symple
{
	static const TypeNode* sBooleanType = new TypeNode(Type::PrimitiveType::Bool, new TypeModifiersNode({}));

	class BooleanLiteralExpressionNode : public LiteralExpressionNode
	{
	public:
		BooleanLiteralExpressionNode(const Token* literal)
			: LiteralExpressionNode(sBooleanType, literal) {}

		Kind GetKind() const override
		{
			return Kind::BooleanLiteralExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Boolean Literal Expression (" << mLiteral->GetLex() << ")";

			return ss.str();
		}
	};
}