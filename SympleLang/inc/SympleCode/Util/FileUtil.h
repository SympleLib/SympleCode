#pragma once

#include <cstdio>

namespace Symple
{
	FILE* OpenFile(char* path);
	extern int (*CloseFile)(FILE*);

	std::string ReadFile(FILE*);
}