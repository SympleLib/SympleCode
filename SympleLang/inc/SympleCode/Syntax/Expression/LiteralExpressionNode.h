#pragma once

#include "SympleCode/Syntax/Expression/ExpressionNode.h"

namespace Symple
{
	class LiteralExpressionNode : public ExpressionNode
	{
	public:
		LiteralExpressionNode(std::shared_ptr<Token>);
	};
}