#pragma once

#include "SympleCode/Symbol/TypeSymbol.h"

namespace Symple::Binding
{
	struct CastTable
	{
		static bool CanImplicitelyCast(shared_ptr<Symbol::TypeSymbol> from, shared_ptr<Symbol::TypeSymbol> to);
		static bool CanExplicitelyCast(shared_ptr<Symbol::TypeSymbol> from, shared_ptr<Symbol::TypeSymbol> to);
	};
}