#pragma once

#include "SympleCode/Syntax/MemberSyntax.h"

#include "SympleCode/Binding/Node.h"

namespace Symple::Binding
{
	class BoundStatement : public Node
	{
	public:
		BoundStatement(shared_ptr<Syntax::MemberSyntax> syntax)
			: Node(syntax) {}

		virtual Kind GetKind()
		{ return Statement; }
	};
}