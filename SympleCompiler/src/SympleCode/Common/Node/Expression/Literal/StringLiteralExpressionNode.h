#pragma once

#include "SympleCode/Common/Node/Expression/Literal/LiteralExpressionNode.h"

namespace Symple
{
	static const TypeNode* sStringType = new TypeNode(Type::PrimitiveType::Char, new TypeModifiersNode({}), new TypeContinueNode(new Token(Token::Kind::Asterisk), new TypeModifiersNode({}), nullptr));

	class StringLiteralExpressionNode : public LiteralExpressionNode
	{
	public:
		StringLiteralExpressionNode(const Token* literal)
			: LiteralExpressionNode(sStringType, literal) {}

		Kind GetKind() const override
		{
			return Kind::StringLiteralExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "String Literal Expression (" << mLiteral->GetLex() << ")";

			return ss.str();
		}
	};
}