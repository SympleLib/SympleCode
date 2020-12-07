#pragma once

#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class ExpressionNode : public Node
	{
	public:
		ExpressionNode(const std::list<Node*>& children = {});
	};
}