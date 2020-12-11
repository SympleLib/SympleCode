#include "SympleCode/Emit/Emitter.h"

#include <windows.h>
#include <cstdlib>
#include <cstring>
#include <memory>

namespace Symple
{
	Emitter::Emitter(const char* path)
		: mPath(path), mOpen(false)
	{
		while (OpenFile());
	}

	Emitter::~Emitter()
	{
		if (mOpen)
			fclose(mFile);
		mOpen = false;
	}

	void Emitter::Emit(const CompilationUnitNode* unit)
	{
		
	}

	bool Emitter::OpenFile()
	{
		errno_t err = fopen_s(&mFile, mPath, "w");
		if (err)
		{
			char msg[32];
			if (!strerror_s(msg, err))
			{
				char realMsg[157];
				sprintf_s(realMsg, "[!]: Error opening file '%.100s%.3s': %.25s", mPath, strlen(mPath) > 100 ? "..." : "", msg);
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, realMsg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
			else
			{
				char msg[138];
				sprintf_s(msg, "[!]: Unknown error opening file '%.100s%.3s'", mPath, strlen(mPath) > 100 ? "..." : "");
				MessageBeep(MB_ICONSTOP);
				int instruct = MessageBoxA(NULL, msg, "Error Opening File!", MB_ABORTRETRYIGNORE);
				if (instruct == IDABORT)
					exit(IDABORT);
				else if (instruct == IDIGNORE)
					exit(IDIGNORE);
				else if (instruct == IDRETRY)
					return true;
			}
		}
		else
		{
			mOpen = true;
			return false;
		}

		return true;
	}
}