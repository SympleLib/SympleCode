#pragma once

#include <cstdio>
#include <string>

namespace Symple::Util
{
	FILE* OpenFile(char* path, char* perms);
	extern int (*CloseFile)(FILE*);

	std::string ReadFile(FILE*, unsigned max = -1);
}