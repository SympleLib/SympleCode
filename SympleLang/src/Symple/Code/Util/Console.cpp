#include "Symple/Code/Util/Console.h"

#if defined(_WIN32)
#include <Windows.h>
#endif

namespace Symple::Code
{
	SY_API _Console Console;

	ConsoleColor _Console::GetConsoleColor() const
	{ return col; }

	void _Console::SetConsoleColor(ConsoleColor col)
	{
		if (this->col == col)
			return;

	#if defined(_WIN32)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)col);
	#endif
	}
}