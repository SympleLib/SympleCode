#pragma once

#include "SympleCode/Symbol/Symbol.h"
#include "SympleCode/Symbol/TypeSymbol.h"

namespace Symple::Symbol
{
	class FunctionSymbol : public Symbol
	{
	private:
		shared_ptr<TypeSymbol> mType;
	public:
	};
}