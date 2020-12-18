#include "SympleCode/Parser/Lexer.h"

namespace Symple
{
	Lexer::Lexer(const char* source)
		: mSource(source), mCurrent(source), mLine(1), mColumn(0) {}

	Token* Lexer::Next()
	{
		while (CheckNewLine(Peek()) || ShouldIgnore(Peek()))
			Get();

		char c = Peek();
		if (!c)
			return Atom(Token::Kind::EndOfFile);

		if (IsStartDigit(c))
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
		case '!':
			return Equal();
		case '=':
			return Equal();
		case ';':
			return Atom(Token::Kind::Semicolon);
		case '{':
			return Atom(Token::Kind::OpenBrace);
		case '}':
			return Atom(Token::Kind::CloseBrace);
		case '[':
			return Atom(Token::Kind::OpenBracket);
		case ']':
			return Atom(Token::Kind::CloseBracket);
		case '(':
			return Atom(Token::Kind::OpenParenthesis);
		case ')':
			return Atom(Token::Kind::CloseParenthesis);
		case '<':
			return Atom(Token::Kind::LeftArrow);
		case '>':
			return Atom(Token::Kind::RightArrow);
		case ',':
			return Atom(Token::Kind::Comma);
		case '@':
			return Atom(Token::Kind::At);
		case '#':
			return Comment();
		case '"':
			return String();
		}

		return Atom(Token::Kind::Unknown);
	}
	

	bool Lexer::ShouldIgnore(char c)
	{
		return c == ' ' || c == '\t' || c == '\r' || c == 'Í';
	}

	bool Lexer::CheckNewLine(char c)
	{
		if (c == '\n')
		{
			mLine++;
			mColumn = 0;

			return true;
		}

		return false;
	}

	bool Lexer::IsIdentifier(char c)
	{
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$' || (c >= '0' && c <= '9');
	}

	bool Lexer::IsStartDigit(char c)
	{
		return (c >= '0' && c <= '9');
	}

	bool Lexer::IsDigit(char c)
	{
		return (c >= '0' && c <= '9') || c == 'x' || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
	}

	char Lexer::Peek(int offset) const
	{
		return mCurrent[offset];
	}

	char Lexer::Get()
	{
		mColumn++;
		return *mCurrent++;
	}


	Token* Lexer::Atom(Token::Kind kind)
	{
		return new Token(kind, mCurrent++, 1, mLine, mColumn);
	}

	Token* Lexer::Identifier()
	{
		const char* beg = mCurrent;
		Get();
		while (IsIdentifier(Peek()))
			Get();
		std::string_view identifier(beg, std::distance(beg, mCurrent));
		if (identifier == "hint")
			return new Token(Token::Kind::Hint, beg, mCurrent, mLine, mColumn);
		if (identifier == "true")
			return new Token(Token::Kind::True, beg, mCurrent, mLine, mColumn);
		if (identifier == "false")
			return new Token(Token::Kind::False, beg, mCurrent, mLine, mColumn);
		if (identifier == "return")
			return new Token(Token::Kind::Return, beg, mCurrent, mLine, mColumn);
		if (identifier == "break")
			return new Token(Token::Kind::Break, beg, mCurrent, mLine, mColumn);
		if (identifier == "while")
			return new Token(Token::Kind::While, beg, mCurrent, mLine, mColumn);
		if (identifier == "extern")
			return new Token(Token::Kind::Extern, beg, mCurrent, mLine, mColumn);

		if (identifier == "if")
			return new Token(Token::Kind::If, beg, mCurrent, mLine, mColumn);
		if (identifier == "else")
			return new Token(Token::Kind::Else, beg, mCurrent, mLine, mColumn);
		return new Token(Token::Kind::Identifier, beg, mCurrent, mLine, mColumn);
	}

	Token* Lexer::Comment()
	{
		Get();
		const char* beg = mCurrent;
		while (Peek() != '#')
		{
			Get();
			if (CheckNewLine(Peek(-1)))
				break;
		}
		return new Token(Token::Kind::Comment, beg, mCurrent - 1, mLine, mColumn);
	}

	Token* Lexer::Number()
	{
		const char* beg = mCurrent;
		Get();
		while (IsDigit(Peek()))
			Get();
		return new Token(Token::Kind::Number, beg, mCurrent, mLine, mColumn);
	}

	Token* Lexer::String()
	{
		Get();
		const char* beg = mCurrent;
		while (Peek() != '"' || Peek(-1) == '\\')
			Get();
		Get();
		return new Token(Token::Kind::String, beg, mCurrent - 1, mLine, mColumn);
	}

	Token* Lexer::Equal()
	{
		const char* beg = mCurrent;
		Get();
		if (Peek() == '=')
		{
			if (*beg == '=')
			{
				Get();
				return new Token(Token::Kind::EqualEqual, beg, mCurrent, mLine, mColumn);
			}
			if (*beg == '!')
			{
				Get();
				return new Token(Token::Kind::NotEqual, beg, mCurrent, mLine, mColumn);
			}
		}

		return new Token(Token::Kind::Equal, beg, mCurrent, mLine, mColumn);
	}
}