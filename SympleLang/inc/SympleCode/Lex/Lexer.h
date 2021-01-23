#pragma once

#include <cstdio>
#include <string_view>

#include "SympleCode/Lex/Token/Token.h"

namespace Symple
{
	class Lexer
	{
	private:
		FILE* mFile;
		std::string_view mPath;
		
		unsigned mLine, mColumn;
	public:
		Lexer(std::string_view path);

		Token* Lex();
	private:
		char Peek(unsigned off = 0);

		Token* Identifier();
	};
}