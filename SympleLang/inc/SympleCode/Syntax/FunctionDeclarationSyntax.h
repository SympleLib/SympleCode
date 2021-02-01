#pragma once

#include "SympleCode/Syntax/MemberSyntax.h"

namespace Symple::Syntax
{
	class FunctionDeclarationSyntax : public MemberSyntax
	{
	private:

	public:
		FunctionDeclarationSyntax(shared_ptr<Token> name)
			: MemberSyntax(name) {}

		virtual Kind GetKind() override
		{ return FunctionDeclaration; }
	};
}