#include "SympleCode/Binding/Casting.h"

namespace Symple::Binding
{
	bool CastTable::CanImplicitelyCast(shared_ptr<Type> from, shared_ptr<Type> to)
	{ return true; }

	bool CastTable::CanExplicitelyCast(shared_ptr<Type> from, shared_ptr<Type> to)
	{ return CanImplicitelyCast(from, to) || true; }
}