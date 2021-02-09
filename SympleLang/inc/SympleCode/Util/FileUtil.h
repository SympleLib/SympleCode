#pragma once

#include <cstdio>
#include <string>

namespace Symple::Util
{
	FILE* OpenFile(char* path, char* perms);
	FILE* OpenTempFile();
	extern int (*CloseFile)(FILE*);

	void DumpFile(FILE* from, FILE* to = stdout);
	std::string ReadFile(FILE*, unsigned max = -1);
}