#pragma once

using Token = int8_t;

namespace Tokens
{
	typedef enum : Token
	{
		Number,
		Identifier,
		LeftParen,
		RightParen,
		LeftSquare,
		RightSquare,
		LeftCurly,
		RightCurly,
		LessThan,
		GreaterThan,
		Equal,
		Plus,
		Minus,
		Asterisk,
		Slash,
		Hash,
		Dot,
		Comma,
		Colon,
		Semicolon,
		SingleQuote,
		DoubleQuote,
		Comment,
		Pipe,
		End,
		Unexpected,

		Dollar,
		At,
		Exclimation,
	};

	inline const char* ToString(Token token)
	{
		static const char* names[]{
		"Number",      "Identifier",  "LeftParen",  "RightParen", "LeftSquare",
		"RightSquare", "LeftCurly",   "RightCurly", "LessThan",   "GreaterThan",
		"Equal",       "Plus",        "Minus",      "Asterisk",   "Slash",
		"Hash",        "Dot",         "Comma",      "Colon",      "Semicolon",
		"SingleQuote", "DoubleQuote", "Comment",    "Pipe",       "End",
		"Unexpected", "Dollar", "At", "Exclimation",
		};
		return names[static_cast<int>(token)];
	}
}