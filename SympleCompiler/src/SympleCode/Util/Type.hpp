#pragma once

#include <string>

struct Type
{
	std::string Name;
	long Size;
};

const Type IntType = { "int", sizeof(int) };