#pragma once

#include "SympleCode/Common/Token.h"

namespace Symple
{
	class Lexer
	{
	private:
		const char* mSource;
		const char* mCurrent;

		int mLine, mColumn;
	public:
		Lexer(const char* source = "");

		Token* Next();
	private:
		static bool ShouldIgnore(char c);
		static bool IsIdentifier(char c);
		static bool IsStartDigit(char c);
		static bool IsDigit(char c);

		bool CheckNewLine(char c);

		char Peek(int offset = 0) const;
		char Get();

		Token* Atom(Token::Kind kind);
		Token* Identifier();
		Token* Preprocess();
		Token* Comment();
		Token* Number();
		Token* String();
		Token* Equal();
	};
}