#include "SympleCode/Util/ConsoleColor.h"

#if _WIN32
#include <Windows.h>
#endif

namespace Symple::Util
{
	static ConsoleColor sConsoleColor;

	void SetConsoleColor(ConsoleColor c)
	{
		if (sConsoleColor == c)
			return;

		sConsoleColor = c;

#if _WIN32
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), sConsoleColor);
#endif
	}

	ConsoleColor GetConsoleColor()
	{ return sConsoleColor; }

	void ResetConsoleColor()
	{ SetConsoleColor(ConsoleColor::Reset); }
}