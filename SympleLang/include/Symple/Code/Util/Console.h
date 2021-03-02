#pragma once

#include "Symple/Code/Common.h"

namespace Symple::Code
{
	enum class SYC_API ConsoleColor
	{
		Black,
		DarkBlue,
		DarkGreen,
		DarkCyan,
		DarkRed,
		DarkMagenta,
		DarkYellow,
		Grey,
		DarkGrey,
		Blue,
		Green,
		Cyan,
		Red,
		Magenta,
		Yellow,
		White,

		Reset = White,
	};

	struct SYC_API _Console
	{
		ConsoleColor GetConsoleColor() const;
		void SetConsoleColor(ConsoleColor);

		SY_PROPERTY_GET_SET(GetConsoleColor, SetConsoleColor) ConsoleColor Color;
	private:
		ConsoleColor col;
	};

	extern SYC_API _Console Console;
}