#include "Symple/Common/Common.h"
#include "Symple/Syntax/Lexer.h"

namespace Symple
{
	Lexer::Lexer(const GlobalRef<File> &file)
		: file(file), p(file->Source.c_str()) {}


	GlobalRef<Token> Lexer::Lex()
	{
		//if (uint32 pre = NumberPrefix(p))
		//	return LexNumber(pre);
		//else if (IsIdentifier(Current))
		//	return LexIdentifier();
		//else
		{
			// Single-line comments
			if (StartsWith(p, "//"))
			{
				Next(); Next(); // Eat tokens
				auto *beg = p;
				while (!(Current == '\n' || Current == 0))
					Next();
				return MakeGlobalRef<Token>(TokenKind::SingleLineComment, beg, p, file);
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
				return MakeGlobalRef<Token>(TokenKind::SingleLineComment, beg, end, file);
			}
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

	uint32 Lexer::NumberPrefix(const char *p)
	{
		for (uint32 i = 0; i < sizeof(NumberPrefixes) / sizeof(*NumberPrefixes); i++)
			if (StartsWith(p, NumberPrefixes[i]))
				return i + 1;
		return 0;
	}

	bool Lexer::IsNumber(char)
	{
		return false;
	}

	bool Lexer::IsIdentifier(char)
	{
		return false;
	}

	bool Lexer::IsWhiteSpace(char)
	{
		return false;
	}
}