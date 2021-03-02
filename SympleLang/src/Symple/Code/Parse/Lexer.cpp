#include "Symple/Code/Parse/Lexer.h"

namespace Symple::Code
{
	Lexer::Lexer(const GlobalRef<const File> &file)
		: m_File(file) {}

	TokenList Lexer::LexAll()
	{
		TokenList toks;

		do
			toks.push_back(Lex());
		while (Current);
		return toks;
	}

	GlobalRef<Token> Lexer::Lex()
	{
		while (IsWhiteSpace(Current) || Current == '\n')
		{
			if (Current == '\n')
			{
				m_Line++;
				m_DisplayLine++;
				m_Column = 0;
				m_Pointer++;
			}
			else
				Next();
		}

		if (!Current)
		{
			const char *beg = &Next();
			return MakeToken(TokenKind::EndOfFile, beg, &Current);
		}

		if (IsNumber(Current))
			return LexNumber();
		else if (IsIdentifier(Current))
			return LexIdentifier();
		else
			return LexPunctuation();
	}


	bool Lexer::IsNumber(char c)
	{ return c >= '0' && c <= '9' || c == '.'; }

	bool Lexer::IsIdentifier(char c)
	{ return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9'; }

	bool Lexer::IsWhiteSpace(char c)
	{ return c == ' ' || c == '\t' || c == '\r'; }


	const char &Lexer::Peek()
	{ return *m_Pointer; }

	const char &Lexer::Next()
	{
		const char &prev = Current;
		m_Column++;
		m_Pointer++;
		return prev;
	}


	GlobalRef<Token> Lexer::MakeToken(TokenKind kind, const char *beg, const char *end)
	{ return MakeRef<Token>(kind, beg, end, m_File, m_Line, m_DisplayLine, m_Column); }
}