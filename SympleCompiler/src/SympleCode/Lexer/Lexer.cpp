#include "SympleCode/Lexer/Lexer.h"

namespace Symple
{
	Lexer::Lexer(const char* source)
		: mSource(source), mCurrent(source) {}

	Token* Lexer::Next()
	{
		while (ShouldIgnore(Peek()))
			Get();

		char c = Peek();
		if (!c)
			return Atom(Token::Kind::EndOfFile);

		if (IsNumber(c))
			return Number();
		if (IsIdentifier(c))
			return Identifier();

		return Atom(Token::Kind::Unknown);
	}
	

	bool Lexer::ShouldIgnore(char c)
	{
		return c == ' ' || c == '\t' || c == '\r' || c == '\n';
	}

	bool Lexer::IsIdentifier(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$' || (c >= '0' && c <= '9');
	}

	bool Lexer::IsNumber(char c)
	{
		return (c >= '0' && c <= '9') || c == '.';
	}

	char Lexer::Peek(int step) const
	{
		return mCurrent[step];
	}

	char Lexer::Get()
	{
		return *mCurrent++;
	}


	Token* Lexer::Atom(Token::Kind kind)
	{
		return new Token(kind, mCurrent++, 1);
	}

	Token* Lexer::Identifier()
	{
		const char* beg = mCurrent;
		Get();
		while (IsIdentifier(Peek()))
			Get();
		return new Token(Token::Kind::Identifier, beg, mCurrent);
	}

	Token* Lexer::Number()
	{
		const char* beg = mCurrent;
		Get();
		while (IsNumber(Peek()))
			Get();
		return new Token(Token::Kind::Number, beg, mCurrent);
	}
}