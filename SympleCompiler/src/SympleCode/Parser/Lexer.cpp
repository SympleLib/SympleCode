#include "SympleCode/Parser/Lexer.h"

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

		switch (c)
		{
		case '+':
			return Atom(Token::Kind::Plus);
		case '-':
			return Atom(Token::Kind::Minus);
		case '*':
			return Atom(Token::Kind::Asterisk);
		case '/':
			return Atom(Token::Kind::Slash);
		case '=':
			return Atom(Token::Kind::Equal);
		case ';':
			return Atom(Token::Kind::Semicolon);
		case '{':
			return Atom(Token::Kind::OpenBracket);
		case '}':
			return Atom(Token::Kind::CloseBracket);
		case '(':
			return Atom(Token::Kind::OpenParenthesis);
		case ')':
			return Atom(Token::Kind::CloseParenthesis);
		}

		return Atom(Token::Kind::Unknown);
	}
	

	bool Lexer::ShouldIgnore(char c)
	{
		return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == 'Í';
	}

	bool Lexer::IsIdentifier(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$' || (c >= '0' && c <= '9');
	}

	bool Lexer::IsNumber(char c)
	{
		return (c >= '0' && c <= '9') || c == '.';
	}

	char Lexer::Peek(int offset) const
	{
		return mCurrent[offset];
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
		std::string_view identifier(beg, std::distance(beg, mCurrent));
		if (identifier == "true")
			return new Token(Token::Kind::True, beg, mCurrent);
		if (identifier == "false")
			return new Token(Token::Kind::False, beg, mCurrent);
		if (identifier == "void")
			return new Token(Token::Kind::Void, beg, mCurrent);
		if (identifier == "function")
			return new Token(Token::Kind::Function, beg, mCurrent);
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