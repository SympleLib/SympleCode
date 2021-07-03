#pragma once

#include "Symple/Code/Type/Type.h"
#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	struct SYC_API SymbolBuddy
	{
		std::vector<TypeBaseRef> typeBases;

		SymbolBuddy();

		bool IsTypeBase(GlobalRef<const Token>);
		TypeBaseRef GetTypeBase(GlobalRef<const Token>);
	};
}