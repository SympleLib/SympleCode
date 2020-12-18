#pragma once

#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class ExpressionNode : public Node
	{
	public:
		virtual Kind GetKind() const override
		{
			return Kind::Expression;
		}
	};
}