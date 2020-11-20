#pragma once

#include <string>

struct Type
{
	std::string Name;
	long Size;
};

const Type VoidType = { "void", 0 };
const Type IntType = { "int", sizeof(int) };