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

		virtual void PrintShort(std::ostream& os = std::cout)
		{ os << GetName()->GetText(); }

		shared_ptr<Token> GetName()
		{ return GetToken(); }
	};
}