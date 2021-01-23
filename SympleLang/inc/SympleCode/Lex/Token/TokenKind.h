#pragma once

namespace Symple
{
	enum class TokenKind : unsigned
	{
		EndOfFile,
		Unkown,

		Identifier,
		Number,

		Last = Number,
	};

	extern char* TokenKindNames[(unsigned)TokenKind::Last + 1];
}