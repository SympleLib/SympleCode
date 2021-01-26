#include "SympleCode/Syntax/Lexer.h"

#include "SympleCode/Util/FileUtil.h"

#define Current &Peek()

namespace Symple
{
	Lexer::Lexer(char* file)
		: mFile(file), mSource(), mPosition(), mLine(), mColumn()
	{
		FILE* fs = OpenFile(file, "rb");
		if (fs)
		{
			mSource = ReadFile(fs);
			CloseFile(fs);
		}
	}

	Lexer::Lexer(char* file, std::string& source)
		: mFile(file), mSource(source), mPosition(), mLine(), mColumn()
	{}

	
	std::shared_ptr<Token> Lexer::Lex()
	{
		while (CheckNewLine() || IsWhiteSpace(Peek()))
			Next();

		char c = Peek();
		if (!c)
			return LexAtom(Token::EndOfFile);

		if (IsNumber(c))
			return LexNumber();
		if (IsIdentifier(c))
			return LexIdentifier();

		switch (c)
		{
		case ';':
			return LexAtom(Token::Semicolon);
		case '=':
			return LexAtom(Token::Equal);
		default:
			return LexAtom(Token::Unknown);
		}
	}


	std::string_view Lexer::GetSource()
	{ return mSource; }

	char* Lexer::GetFile()
	{ return mFile; }


	bool Lexer::IsWhiteSpace(char c)
	{
		return c == ' ' || c == '\t' || c == '\r' || c == 0xCC;
	}

	bool Lexer::IsIdentifier(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$' || (c >= '0' && c <= '9');
	}

	bool Lexer::IsNumber(char c)
	{
		return c >= '0' && c <= '9';
	}

	bool Lexer::CheckNewLine()
	{
		if (Peek() == '\n')
		{
			mLine++;
			mColumn = 0;

			return true;
		}
		else
			return false;
	}


	char& Lexer::Peek(unsigned off)
	{
		unsigned pos = mPosition + off;
		if (pos >= mSource.length())
			return mSource[mSource.length()];
		return mSource[pos];
	}

	char& Lexer::Next()
	{
		char& prev = mSource[mPosition];
		mColumn++;
		mPosition++;
		if (mPosition > mSource.length())
		{
			mColumn--;
			mPosition--;
		}
		return prev;
	}


	std::shared_ptr<Token> Lexer::LexAtom(Token::Kind kind)
	{ return std::make_shared<Token>(kind, &Next(), 1, mLine, mColumn, mFile); }

	std::shared_ptr<Token> Lexer::LexIdentifier()
	{
		char* beg = Current;
		int column = mColumn;
		Next();
		while (IsIdentifier(Peek()))
			Next();

		return std::make_shared<Token>(Token::Identifier, beg, Current, mLine, column, mFile);
	}

	std::shared_ptr<Token> Lexer::LexNumber()
	{
		char* beg = Current;
		int column = mColumn;
		Next();
		while (IsNumber(Peek()))
			Next();

		return std::make_shared<Token>(Token::Number, beg, Current, mLine, column, mFile);
	}
}