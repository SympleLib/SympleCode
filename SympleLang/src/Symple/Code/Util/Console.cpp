#include "Symple/Code/Util/Console.h"

#if defined(_WIN32)
#include <Windows.h>
#endif

namespace Symple::Code
{
	ConsoleColor tmp;
	SYC_API _Console &Console = *(_Console *)&tmp;

	void _Console::SetColor(ConsoleColor col)
	{
		if (tmp == col)
			return;

		tmp = col;

	#if defined(_WIN32)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)col);
	#endif
	}
}