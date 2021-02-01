#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple::Syntax
{
	class StatementSyntax : public Node
	{
	public:
		StatementSyntax(shared_ptr<Token> tok)
			: Node(tok) {}

		virtual Kind GetKind() override
		{ return Statement; }
	};
}