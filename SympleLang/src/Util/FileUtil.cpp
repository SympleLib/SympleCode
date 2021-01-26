#include "SympleCode/Util/FileUtil.h"

#include <spdlog/spdlog.h>

namespace Symple::Util
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


	std::string ReadFile(FILE* fs, unsigned max)
	{
		if (!fs)
			return nullptr;

		fseek(fs, 0, SEEK_END);
		unsigned allocSz = std::min((unsigned)(ftell(fs) + 1), max);
		char* src = new char[allocSz];
		src[allocSz - 1] = 0;
		rewind(fs);
		fread_s(src, allocSz - 1, 1, allocSz - 1, fs);

		std::string str(src);
		delete[] src;

		return str;
	}
}