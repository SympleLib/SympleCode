#include "SympleCode/Binding/Casting.h"

namespace Symple::Binding
{
	bool CastTable::CanImplicitelyCast(shared_ptr<Symbol::TypeSymbol> from, shared_ptr<Symbol::TypeSymbol> to)
	{ return true; }

	bool CastTable::CanExplicitelyCast(shared_ptr<Symbol::TypeSymbol> from, shared_ptr<Symbol::TypeSymbol> to)
	{ return CanImplicitelyCast(from, to) || true; }
}