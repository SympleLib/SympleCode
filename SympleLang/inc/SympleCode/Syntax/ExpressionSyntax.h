#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple::Syntax
{
	class ExpressionSyntax : public Node
	{
	public:
		ExpressionSyntax(shared_ptr<Token> tok)
			: Node(tok) {}

		virtual Kind GetKind() override
		{ return Expression; }
	};
}