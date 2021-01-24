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

		std::shared_ptr<Token> Lex();

		std::string_view GetSource();
		char* GetFile();
	private:
		static bool IsWhiteSpace(char);
		static bool IsIdentifier(char);
		static bool IsNumber(char);
		bool CheckNewLine();

		char& Peek(unsigned off = 0);
		char& Next();

		std::shared_ptr<Token> LexAtom(Token::Kind);
		std::shared_ptr<Token> LexIdentifier();
		std::shared_ptr<Token> LexNumber();
	};
}