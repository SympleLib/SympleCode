#pragma once

#include "SympleCode/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class ModifiableExpressionNode : public ExpressionNode
	{
	public:
		ModifiableExpressionNode(const TypeNode* type)
			: ExpressionNode(type) {}

		Kind GetKind() const override
		{
			return Kind::ModifiableExpression;
		}

		virtual bool IsMutable() const = 0;
	};
}