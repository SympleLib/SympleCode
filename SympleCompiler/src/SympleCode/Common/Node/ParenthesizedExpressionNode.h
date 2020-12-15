#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/ExpressionNode.h"

namespace Symple
{
	class ParenthesizedExpressionNode : public ExpressionNode
	{
	private:
		const Token* mOpen;
		const ExpressionNode* mExpression;
		const Token* mClose;
	public:
		ParenthesizedExpressionNode(const Token* open, const ExpressionNode* expression, const Token* close)
			: mOpen(open), mExpression(expression), mClose(close) {}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const ExpressionNode* GetExpression() const
		{
			return mExpression;
		}

		const Token* GetClose() const
		{
			return mClose;
		}
	};
}