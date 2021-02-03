#pragma once

#include "SympleCode/Syntax/StatementSyntax.h"

#include "SympleCode/Binding/Node.h"

namespace Symple::Binding
{
	class BoundStatement : public Node
	{
	public:
		BoundStatement(shared_ptr<Syntax::StatementSyntax> syntax)
			: Node(syntax) {}

		virtual Kind GetKind()
		{ return Statement; }
	};
}