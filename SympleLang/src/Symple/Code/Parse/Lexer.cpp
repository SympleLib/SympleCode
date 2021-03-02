#include "Symple/Code/Parse/Lexer.h"

namespace Symple::Code
{
	Lexer::Lexer(const GlobalRef<const File> &file)
		: m_File(file), m_Pointer(file->Source.c_str()) {}

	TokenList Lexer::LexAll()
	{
		TokenList toks;

		do
			toks.push_back(Lex());
		while (Current);
		toks.push_back(Lex());
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

	GlobalRef<Token> Lexer::LexNumber()
	{
		const char *beg = &Next();
		while (IsNumber(Current))
			Next();
		return MakeToken(TokenKind::Number, beg, &Current);
	}
	
	#define KEYWORD(key, word) if (identifier == #key) return MakeToken(TokenKind::##word##Keyword, beg, &Current)
	GlobalRef<Token> Lexer::LexIdentifier()
	{
		const char *beg = &Next();
		while (IsIdentifier(Current))
			Next();

		std::string_view identifier(beg, std::distance(beg, &Current));
		KEYWORD(void, Void);
		KEYWORD(byte, Byte);
		KEYWORD(short, Short);
		KEYWORD(int, Int);
		KEYWORD(long, Long);

		KEYWORD(ret, Return);
		KEYWORD(return, Return);
		
		return MakeToken(TokenKind::Identifier, beg, &Current);
	}
	#undef KEYWORD

	GlobalRef<Token> Lexer::LexPunctuation()
	{
		const char *beg = &Current;

		for (uint32 i = 0; i < sizeof(s_Punctuators) / sizeof(*s_Punctuators); i++)
			if (!strncmp(&Current, s_Punctuators[i], strlen(s_Punctuators[i])))
			{
				for (uint32 j = 0; j < strlen(s_Punctuators[i]); j++)
					Next();
				return MakeToken((TokenKind)((uint32)TokenKind::Punctuator + i), beg, &Current);
			}

		Next();
		return MakeToken(TokenKind::Unknown, beg, &Current);
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