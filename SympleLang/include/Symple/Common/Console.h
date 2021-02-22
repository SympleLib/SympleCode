#pragma once

#include "Symple/Common/Common.h"

namespace Symple
{
	enum class ConsoleColor
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

	struct SY_API _Console
	{
		ConsoleColor GetConsoleColor() const;
		void SetConsoleColor(ConsoleColor);

		SY_PROPERTY_GET_SET(GetConsoleColor, SetConsoleColor) ConsoleColor Color;
	private:
		ConsoleColor col;
	};

	extern SY_API _Console Console;
}