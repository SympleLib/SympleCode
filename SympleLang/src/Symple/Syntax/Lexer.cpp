#include "Symple/Common/Common.h"
#include "Symple/Syntax/Lexer.h"

namespace Symple
{
	Lexer::Lexer(const GlobalRef<File> &file)
		: file(file), p(file->Source.c_str()) {}


	GlobalRef<Token> Lexer::Lex()
	{
		bool isNewLine;
		while ((isNewLine = Current == '\n') || IsWhiteSpace(Current))
		{
			if (isNewLine)
			{
				ln++;
				disLn++;
				col = 0;
				p++;
			}
			else
				Next();
		}

		if (uint32 pre = NumberPrefix(p))
			return LexNumber(pre);
		else if (IsIdentifier(Current))
			return LexIdentifier();
		else
		{
			if (Current == 0)
				return MakeToken(TokenKind::EndOfFile, --p, ++p); // Arguments gets push on the stack from right to left, so I can't do 'p++, p'

			// Single-line comments
			if (StartsWith(p, "//"))
			{
				Next(); Next(); // Eat tokens
				auto *beg = p;
				while (Current != '\n' && Current != 0)
					Next();
				return MakeToken(TokenKind::SingleLineComment, beg, p);
			}

			// Multi-line comments
			if (StartsWith(p, "/*"))
			{
				Next(); Next(); // Eat tokens
				auto *beg = p;
				while (!StartsWith(p, "*/"))
					Next();
				auto *end = p;
				Next(); Next(); // Eat tokens
				return MakeToken(TokenKind::SingleLineComment, beg, end);
			}

			return LexPunctuation();
		}
	}


	const char &Lexer::Peek()
	{ return *p; }

	const char &Lexer::Peek(uint32 off)
	{
		const char *c = p + off;
		if (c > file->Source.cend().operator->())
			return *file->Source.cend().operator->();
		return *c;
	}

	const char &Lexer::Next()
	{
		const char &prev = Current;
		col++;
		p++;
		return prev;
	}


	GlobalRef<Token> Lexer::MakeToken(TokenKind kind, const char *beg, const char *end)
	{ return MakeGlobalRef<Token>(kind, beg, end, file, ln, disLn, col); }


	GlobalRef<Token> Lexer::LexNumber(uint32 prefix)
	{
		const char *beg = p;
		for (uint32 i = 0; i < strlen(NumberPrefixes[prefix]); i++)
			Next();
		while (IsNumber(prefix))
			Next();

		return MakeToken(TokenKind::Number, beg, p);
	}

	GlobalRef<Token> Lexer::LexIdentifier()
	{
		const char *beg = p;
		while (IsIdentifier(Current))
			Next();

		return MakeToken(TokenKind::Identifier, beg, p);
	}

	GlobalRef<Token> Lexer::LexPunctuation()
	{
		for (uint32 i = 0; i < sizeof(Punctuators) / sizeof(*Punctuators); i++)
			if (StartsWith(p, Punctuators[i]))
			{
				const char *beg = p;
				for (uint32 j = 0; j < strlen(Punctuators[i]); j++)
					Next();

				return MakeToken((TokenKind)((uint32)TokenKind::Punctuator + i), beg, p);
			}

		const char *beg = &Next();
		return MakeToken(TokenKind::Unknown, beg, p);
	}


	uint32 Lexer::NumberPrefix(const char *p)
	{
		for (uint32 i = 0; i < sizeof(NumberPrefixes) / sizeof(*NumberPrefixes); i++)
			if (StartsWith(p, NumberPrefixes[i]))
				return i + 2;
		return !!IsNumber(*p);
	}

	bool Lexer::IsNumber(char c)
	{ return c >= '0' && c <= '9' || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F'; }

	bool Lexer::IsIdentifier(char c)
	{ return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || IsNumber(c); }

	bool Lexer::IsWhiteSpace(char c)
	{ return c == ' ' || c == '\t' || c == '\r'; }
}