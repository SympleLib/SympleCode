#pragma once

#include <cstdint>

#include "SympleCode/Type.hpp"

namespace Symple::VariebleKeeper
{
	void BeginScope();
	void EndScope();

	uint64_t Scope(char* name, const Type& ty);
	uint64_t Get(char* name);
}