#pragma once

#include "Symple/Syntax/Token.h"

namespace Symple
{
	class SY_API Lexer
	{
	private:
		const char *p;
		const GlobalRef<File> file;

		// Line, DisplayLine, Column
		uint32 ln {}, disLn {}, col {};
	public:
		Lexer(const GlobalRef<File> &);

		GlobalRef<Token> Lex();
	private:
		const char &Peek(uint32 offset = 0);
		const char &Next();

		SY_PROPERTY_GET(Peek) const char &Current;

		GlobalRef<Token> LexNumber();
		GlobalRef<Token> LexIdentifier();
		GlobalRef<Token> LexPunctuation();

		static int8 NumberPrefix(const std::string&);
		static bool IsNumber(char);
		static bool IsIdentifier(char);
		static bool IsWhiteSpace(char);

		static constexpr const char *const NumberPrefixes[] =
		{
			"0b",
			"0x",
			"0",
		};
	};
}