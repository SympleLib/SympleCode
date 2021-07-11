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

	struct SYC_API _Console;
	extern SYC_API _Console &Console;
	struct SYC_API _Console
	{
		_Console() = delete;

		void SetColor(ConsoleColor);

		class
		{
		private:
			ConsoleColor value;
		public:
			bool operator ==(ConsoleColor col)
			{ value == col; }

			ConsoleColor operator =(ConsoleColor col)
			{
				Console.SetColor(col);
				return value;
			}

			operator ConsoleColor()
			{ value; }
		} color;
	};
}