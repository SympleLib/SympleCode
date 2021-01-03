#pragma once

#include "SympleCode/Common/Node/Expression/Literal/LiteralExpressionNode.h"

namespace Symple
{
	static const TypeNode* sNullType = new TypeNode(Type::PrimitiveType::Void, new TypeModifiersNode({}), new TypeContinueNode(new Token(Token::Kind::Asterisk), new TypeModifiersNode({}), nullptr));

	class NullLiteralExpressionNode : public LiteralExpressionNode
	{
	public:
		NullLiteralExpressionNode(const Token* literal)
			: LiteralExpressionNode(sNullType, literal) {}

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