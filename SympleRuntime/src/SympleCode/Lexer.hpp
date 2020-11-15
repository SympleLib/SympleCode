#pragma once

#include <string>
#include <functional>

#include "SympleCode/Token.hpp"

namespace Lexer
{
	bool IsDigit(char);
	bool IsIdentifier(char);
	bool IsSpace(char);

	char Peek();
	char Get();

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
	private:
		Token mToken;
		std::string_view mLex;
	};

	void Lex(const char*, std::function<bool(const TokenInfo&)>);
	TokenInfo Next();

	TokenInfo Atom(Token);
	TokenInfo Identifier();
	TokenInfo Number();
	TokenInfo Comment();
}