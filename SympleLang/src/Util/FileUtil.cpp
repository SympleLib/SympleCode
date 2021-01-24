#include "SympleCode/Util/FileUtil.h"

#include <spdlog/spdlog.h>

namespace Symple
{
	FILE* OpenFile(char* path, char* perms)
	{
		FILE* fs;
		errno_t err = fopen_s(&fs, path, perms);
		if (err || !fs)
		{
			char errMsg[32];
			if (strerror_s(errMsg, err))
				spdlog::error("Error Opening File '{}'", path);
			else
				spdlog::error("Error Opening File '{}': {}", path, errMsg);

			return nullptr;
		}
		else
			return fs;
	}

	int (*CloseFile)(FILE*) = fclose;


	std::string ReadFile(FILE* fs)
	{
		if (!fs)
			return nullptr;

		fseek(fs, 0, SEEK_END);
		unsigned sz = ftell(fs);
		char* src = new char[sz + 1];
		src[sz] = 0;
		fread_s(src, sz, 1, sz, fs);

		std::string str(src);
		delete[] src;

		return str;
	}
}