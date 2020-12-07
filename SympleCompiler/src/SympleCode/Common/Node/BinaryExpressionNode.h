#pragma once

#include "SympleCode/Common/Node/ExpressionNode.h"
#include "SympleCode/Common/Operator.h"

namespace Symple
{
	class BinaryExpressionNode : public ExpressionNode
	{
	private:
		const Operator mOperator;
		const ExpressionNode* mLeft;
		const ExpressionNode* mRight;
	public:
		BinaryExpressionNode(Operator oqerator, const ExpressionNode* left, const ExpressionNode* right)
			: mOperator(oqerator), mLeft(left), mRight(right) {}

		inline Kind GetKind() const override
		{
			return Kind::BinaryExpression;
		}

		inline Operator GetOperator() const
		{
			return mOperator;
		}

		inline const ExpressionNode* GetLeft() const
		{
			return mLeft;
		}

		inline const ExpressionNode* GetRight() const
		{
			return mRight;
		}
	};
}