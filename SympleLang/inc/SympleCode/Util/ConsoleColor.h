#pragma once

namespace Symple::Util
{
	enum ConsoleColor
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

	void SetConsoleColor(ConsoleColor c);
	ConsoleColor GetConsoleColor();
	void ResetConsoleColor();
}