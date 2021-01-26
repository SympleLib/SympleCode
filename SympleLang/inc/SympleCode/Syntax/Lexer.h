#pragma once

#include <string>

#include "SympleCode/Syntax/Token.h"

namespace Symple
{
	class Lexer
	{
	private:
		std::string mSource;
		unsigned mPosition;

		char* mFile;
		unsigned mLine, mColumn;
	public:
		Lexer(char* mFile);
		Lexer(char* mFile, std::string& mSource);

		shared_ptr<Token> Lex();

		shared_ptr<Token> LexAtom(Token::Kind);
		shared_ptr<Token> LexIdentifier();
		shared_ptr<Token> LexNumber();

		static bool IsWhiteSpace(char);
		static bool IsIdentifier(char);
		static bool IsNumber(char);

		std::string_view GetSource();
		char* GetFile();
	private:
		bool CheckNewLine();

		char& Peek(unsigned off = 0);
		char& Next();
	};
}