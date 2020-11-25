#include <pch.hpp>
#include "SympleCode/Program.hpp"

#include <cstdio>
#include <cstring>

namespace Symple::Program
{
	uint8_t* sProgram;
	uint64_t sProgramLen;

	void Run(char* path)
	{
		FILE* fs = nullptr;
		errno_t err = 0;
		if (!(err = fopen_s(&fs, path, "rb")) && fs)
		{
			fseek(fs, 0, SEEK_END);
			sProgramLen = ftell(fs);
			delete[] sProgram;
			sProgram = new uint8_t[sProgramLen];
			rewind(fs);
			fread(sProgram, 1, sProgramLen, fs);
		}
		else
		{
			char errMsg[16];
			if (strerror_s(errMsg, err))
				fprintf(stderr, "Error opening file '%s': %s", path, errMsg);
		}
	}
}