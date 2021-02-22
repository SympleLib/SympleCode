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
		const char &Peek();
		const char &Peek(uint32 offset);
		const char &Next();

		SY_PROPERTY_GET(Peek) const char &Current;

		GlobalRef<Token> MakeToken(TokenKind, const char *beg, const char *end);

		GlobalRef<Token> LexNumber(uint32 prefix);
		GlobalRef<Token> LexIdentifier();
		GlobalRef<Token> LexPunctuation();

		static uint32 NumberPrefix(const char *);
		static bool IsNumber(char);
		static bool IsIdentifier(char);
		static bool IsWhiteSpace(char);

		static constexpr const char *const NumberPrefixes[] =
		{
			"0b",
			"0x",
			"0",
		};

		static constexpr const char *const Punctuators[] =
		{
			"+", "-", "*", "/", "%",
		};
	};
}