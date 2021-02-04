#pragma once

#include <cstdio>
#include <string>

namespace Symple::Util
{
	FILE* OpenFile(char* path, char* perms);
	FILE* OpenTempFile();
	extern int (*CloseFile)(FILE*);

	std::string ReadFile(FILE*, unsigned max = -1);
}