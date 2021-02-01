#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple::Syntax
{
	class TypeSyntax : public Node
	{
	public:
		TypeSyntax(shared_ptr<Token> name)
			: Node(name) {}

		virtual Kind GetKind() override
		{ return Type; }

		shared_ptr<Token> GetName()
		{ return GetToken(); }
	};
}