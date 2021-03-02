#pragma once

#include <vector>

#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	class SYC_API Lexer
	{
	private:
		const char *m_Pointer;
		const GlobalRef<const File> m_File;
		uint32 m_Line, m_DisplayLine, m_Column;
	public:
		Lexer(const GlobalRef<const File> &);
		TokenList LexAll();
		GlobalRef<Token> Lex();
	private:
		const char &Peek();
		const char &Next();

		SY_PROPERTY_GET(Peek) const char &Current;

		GlobalRef<Token> MakeToken(TokenKind, const char *beg, const char *end);

		GlobalRef<Token> LexNumber();
		GlobalRef<Token> LexIdentifier();
		GlobalRef<Token> LexPunctuation();

		static bool IsNumber(char);
		static bool IsIdentifier(char);
		static bool IsWhiteSpace(char);

		static constexpr const char *const s_Punctuators[] =
		{
			"+", "-", "*", "/", "%",
			"=", "=>",
			"(", ")",
			";",
		};
	};
}