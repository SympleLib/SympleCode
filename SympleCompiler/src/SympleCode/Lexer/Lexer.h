#pragma once

#include "SympleCode/Common/Token.h"

namespace Symple
{
	class Lexer
	{
	private:
		const char* mSource;
		const char* mCurrent;
	public:
		Lexer(const char* source = "");

		Token* Next();
	private:
		static bool ShouldIgnore(char c);
		static bool IsIdentifier(char c);
		static bool IsNumber(char c);

		char Peek(int step = 0) const;
		char Get();

		Token* Atom(Token::Kind kind);
		Token* Identifier();
		Token* Number();
	};
}