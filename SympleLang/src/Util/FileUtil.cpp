#include <pch.h>
#include "SympleCode/Util/FileUtil.h"

namespace Symple
{
	void OpenFile(FILE*& file, std::string_view path)
	{
		errno_t err = fopen_s(&file, path.data(), "r");
		if (!(err || file))
		{
			char msg[32];
			if (strerror_s(msg, err))
				;
		}
	}

	void CloseFile(FILE* file)
	{

	}
}