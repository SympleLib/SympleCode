#pragma once

#include <string>
#include <unordered_map>

using Token = size_t;
using KeyWord = size_t;

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
	};

	inline const char* ToString(Token token)
	{
		static const char* names[]{
		"Number",      "Identifier",  "LeftParen",  "RightParen", "LeftSquare",
		"RightSquare", "LeftCurly",   "RightCurly", "LessThan",   "GreaterThan",
		"Equal",       "Plus",        "Minus",      "Asterisk",   "Slash",
		"Hash",        "Dot",         "Comma",      "Colon",      "Semicolon",
		"SingleQuote", "DoubleQuote", "Comment",    "Pipe",       "End",
		"Unexpected",
		};
		return names[static_cast<int>(token)];
	}
}