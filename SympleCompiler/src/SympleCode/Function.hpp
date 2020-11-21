#pragma once

#include <string>
#include <vector>

#include "SympleCode/Type.hpp"

typedef struct Param
{
	const Type Type;
	const std::string Name;
} Parameter;

typedef struct Function
{
	const std::string Name;
	const std::vector<Param> Params;
	const Type Return;
} Func;