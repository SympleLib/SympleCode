#include "SympleCode/Emit/Emitter.h"

#include <Windows.h>

#define Emit(fmt, ...) fprintf(mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(mLiteralFile, fmt "\n", __VA_ARGS__)

namespace Symple
{
	Emitter::Emitter(const char* path)
		: mPath(path), mFile(), mLiteralFile(), mData(), mReturn(), mStack(),
			mReturning(), mRegisterManager(new RegisterManager(this))
	{
		while (OpenFile());
		while (OpenLiteralFile());
	}

	Emitter::~Emitter()
	{
		rewind(mLiteralFile);

		char c;
		while ((c = getc(mLiteralFile)) != EOF)
			putc(c, mFile);

		fclose(mLiteralFile);
		fclose(mFile);
	}

	void Emitter::EmitCompilationUnit(const CompilationUnitNode* unit)
	{

	}

	char Emitter::Suff(int sz)
	{
		switch (sz)
		{
		case 1:
			return 'b';
		case 2:
			return 'w';
		case 4:
			return 'l';
		case 8:
			return 'q';
		}

		return 0;
	}

	void Emitter::Push(Register reg, int sz)
	{
		Emit("\tpush%c   %s", Suff(sz), RegisterManager::GetRegister(reg, sz));
	}

	void Emitter::Pop(Register reg, int sz)
	{
		Emit("\tpop%c    %s", Suff(sz), RegisterManager::GetRegister(reg, sz));
	}

	bool Emitter::OpenFile()
	{
		errno_t err = fopen_s(&mFile, mPath, "w");
		if (err && !mFile)
		{
			char msg[32];
			if (!strerror_s(msg, err))
			{
				char realMsg[56];
				sprintf_s(realMsg, "[!]: Error opening temp file: %.25s", msg);
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
				char msg[37] = "[!]: Unknown error opening temp file";
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
			return false;
		}

		return true;
	}

	bool Emitter::OpenLiteralFile()
	{
		errno_t err = tmpfile_s(&mLiteralFile);
		if (err && !mLiteralFile)
		{
			char msg[32];
			if (!strerror_s(msg, err))
			{
				char realMsg[56];
				sprintf_s(realMsg, "[!]: Error opening temp file: %.25s", msg);
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
				char msg[37] = "[!]: Unknown error opening temp file";
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
			return false;
		}

		return true;
	}
}