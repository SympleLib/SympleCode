#pragma once

#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	struct CastTable
	{
		static bool CanImplicitelyCast(shared_ptr<Type> from, shared_ptr<Type> to);
		static bool CanExplicitelyCast(shared_ptr<Type> from, shared_ptr<Type> to);
	};
}