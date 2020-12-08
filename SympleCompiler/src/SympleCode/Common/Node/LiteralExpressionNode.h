#pragma once

#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/LiteralType.h"

namespace Symple
{
	class LiteralExpressionNode : public ExpressionNode
	{
	private:
		const Token* mLiteral;
		LiteralType mType;
	public:
		LiteralExpressionNode(LiteralType type, const Token* literal)
			: mType(type), mLiteral(literal) {}

		inline LiteralType GetType() const
		{
			return mType;
		}

		inline const Token* GetLiteral() const
		{
			return mLiteral;
		}
	};
}