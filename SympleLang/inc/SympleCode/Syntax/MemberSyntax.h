#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple::Syntax
{
	class MemberSyntax : public Node
	{
	public:
		MemberSyntax(shared_ptr<Token> tok)
			: Node(tok) {}

		virtual Kind GetKind() override
		{ return Member; }
	};
}