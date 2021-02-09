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
				spdlog::error("Error opening file '{}'", path);
			else
				spdlog::error("Error opening file '{}': {}", path, errMsg);

			return nullptr;
		}
		else
			return fs;
	}

	FILE* OpenTempFile()
	{
		FILE* fs;
		errno_t err = tmpfile_s(&fs);
		if (err || !fs)
		{
			char errMsg[32];
			if (strerror_s(errMsg, err))
				spdlog::error("Error opening temp file");
			else
				spdlog::error("Error opening temp file: {}", errMsg);

			return nullptr;
		}
		else
			return fs;
	}

	int (*CloseFile)(FILE*) = fclose;


	void DumpFile(FILE* from, FILE* to)
	{
		unsigned start = ftell(from);

		char c;
		while ((c = fgetc(from)) != EOF)
			fputc(c, to);

		fseek(from, start, SEEK_SET);
	}

	std::string ReadFile(FILE* fs, unsigned max)
	{
		if (!fs)
			return {};

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