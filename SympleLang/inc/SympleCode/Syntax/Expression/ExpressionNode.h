#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple
{
	class ExpressionNode : public Node
	{
	protected:
		ExpressionNode(Kind, std::shared_ptr<Token>);
	public:
		ExpressionNode(std::shared_ptr<Token>);
	};
}