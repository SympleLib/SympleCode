#include <pch.h>
#include "SympleCode/Lex/Lexer.h"

#include <cstring>

namespace Symple
{
	Lexer::Lexer(std::string_view path)
		: mPath(path), mLine(), mColumn()
	{
		OpenFile(mFile, mPath);
		mSource = ReadFile(mFile);
	}

	Ref<Token> Lexer::Lex()
	{
		CheckWhiteSpace();

		char c = Peek();


	}


	bool IsIdentifier(char c)
	{

	}


	char Lexer::Next()
	{
		mColumn++;
		return *mSource++;
	}

	char Lexer::Peek(unsigned off)
	{
		return mSource[off];
	}


	void Lexer::CheckWhiteSpace()
	{
		char c = Next();
		while (c == ' ' || c == '\t' || c == '\r' || c == 0xCC || c == '\n')
		{
			if (c == '\n')
			{
				mLine++;
				mColumn = 0;
			}

			c = Next();
		}
	}

	Ref<Token> Lexer::Atom(TokenKind kind)
	{
		return MakeRef<Token>(kind, std::string_view(mSource++, 1), mFile, mLine, mColumn);
	}
}