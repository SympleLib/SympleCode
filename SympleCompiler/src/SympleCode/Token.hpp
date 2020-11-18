#pragma once

#include <string>

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

class TokenInfo
{
public:
	TokenInfo(Token token)
		: mToken{ token }
	{}

	TokenInfo(Token token, const char* beg, const char* end)
		: mToken{ token }, mLex(beg, std::distance(beg, end))
	{}

	TokenInfo(Token token, const char* beg, size_t len)
		: mToken{ token }, mLex(beg, len)
	{}

	inline bool Is(Token token) const
	{
		return mToken == token;
	}

	inline bool IsNot(Token token) const
	{
		return mToken == token;
	}

	inline bool IsEither(Token t1, Token t2) const
	{
		return Is(t1) || Is(t2);
	}

	template<typename... T>
	inline bool IsEither(Token t1, Token t2, Token t) const
	{
		return Is(t1) || IsEither(t1, t...);
	}

	inline Token GetToken() const
	{
		return mToken;
	}

	inline void SetToken(Token token)
	{
		mToken = token;
	}

	inline std::string_view GetLex() const
	{
		return mLex;
	}

	inline void SetLex(std::string_view lex)
	{
		mLex = std::move(lex);
	}

	inline operator std::string_view() const
	{ return mLex; }
private:
	Token mToken;
	std::string_view mLex;
};