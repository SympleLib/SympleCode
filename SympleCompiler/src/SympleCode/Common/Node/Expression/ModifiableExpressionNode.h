#pragma once

#include "SympleCode/Common/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class ModifiableExpressionNode : public ExpressionNode
	{
	public:
		Kind GetKind() const override
		{
			return Kind::ModifiableExpression;
		}
	};
}