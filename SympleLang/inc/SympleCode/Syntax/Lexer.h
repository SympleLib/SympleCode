#pragma once

#include "SympleCode/Syntax/Token.h"

namespace Symple
{
	class Lexer
	{
	private:
		std::string_view mSource;
		unsigned mPosition;

		char* mFile;
		unsigned mLine, mColumn;
	public:
		Lexer(char* mFile);

		std::shared_ptr<Token> Lex();

		std::string_view GetSource();
		char* GetFile();
	private:
		static bool IsWhiteSpace(char);
		static bool IsIdentifier(char);

		char Peek(unsigned off = 0);
		char Get();

		
	};
}