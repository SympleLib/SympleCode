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
		char* mSource;
		unsigned mSourceSize;
		
		unsigned mLine, mColumn;
	public:
		Lexer(std::string_view path);

		Ref<Token> Lex();
	private:
		static bool IsIdentifier(char);

		char Next();
		char Peek(unsigned off = 0);

		void CheckWhiteSpace();
		Ref<Token> Atom(TokenKind kind);
		Ref<Token> Identifier();
	};
}