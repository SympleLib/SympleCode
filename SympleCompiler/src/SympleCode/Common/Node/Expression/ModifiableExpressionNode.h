#pragma once

#include "SympleCode/Common/Node/Expression/ExpressionNode.h"

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
	};
}