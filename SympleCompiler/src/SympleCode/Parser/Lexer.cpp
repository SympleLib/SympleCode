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
		case '=':
			return Equal();
		case '!':
			return Equal();
		case '+':
			return Equal();
		case '-':
			return Equal();
		case '*':
			return Equal();
		case '/':
			return Equal();
		case '%':
			return Equal();
		case '<':
			return Equal();
		case '>':
			return Equal();
		case '\\':
			return Comment();
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
		case ',':
			return Atom(Token::Kind::Comma);
		case '.':
			return Atom(Token::Kind::Period);
		case '@':
			return Atom(Token::Kind::At);
		case '#':
			return Preprocess();
		case '"':
			return String();
		case '\'':
			return Character();
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
		int bColumn = mColumn;
		Get();
		while (IsIdentifier(Peek()))
			Get();
		std::string_view identifier(beg, std::distance(beg, mCurrent));
		if (identifier == "hint")
			return new Token(Token::Kind::Hint, beg, mCurrent, mLine, bColumn);
		if (identifier == "true")
			return new Token(Token::Kind::True, beg, mCurrent, mLine, bColumn);
		if (identifier == "false")
			return new Token(Token::Kind::False, beg, mCurrent, mLine, bColumn);
		if (identifier == "return")
			return new Token(Token::Kind::Return, beg, mCurrent, mLine, bColumn);
		if (identifier == "break")
			return new Token(Token::Kind::Break, beg, mCurrent, mLine, bColumn);
		if (identifier == "while")
			return new Token(Token::Kind::While, beg, mCurrent, mLine, bColumn);
		if (identifier == "extern")
			return new Token(Token::Kind::Extern, beg, mCurrent, mLine, bColumn);

		if (identifier == "if")
			return new Token(Token::Kind::If, beg, mCurrent, mLine, bColumn);
		if (identifier == "else")
			return new Token(Token::Kind::Else, beg, mCurrent, mLine, bColumn);
		return new Token(Token::Kind::Identifier, beg, mCurrent, mLine, bColumn);
	}

	Token* Lexer::Preprocess()
	{
		int bLine = mLine, bColumn = mColumn;
		Get();
		const char* beg = mCurrent;
		while (!(CheckNewLine(Peek()) || Peek() == 0))
		{
			Get();
		}
		Get();
		return new Token(Token::Kind::Preprocess, beg, mCurrent - 1, bLine, bColumn);
	}

	Token* Lexer::Character()
	{
		Get();
		const char* beg = mCurrent;
		int bColumn = mColumn;
		char c = Get();
		if (c == '\\')
		{
			char mc = Get();
			if (Get() != '\'')
				return new Token(Token::Kind::Unknown, beg, mCurrent - 1, mLine, bColumn);
			switch (mc)
			{
			case 'n':
				return new Token(Token::Kind::Character, "\n", 1, mLine, bColumn);
			case 'r':
				return new Token(Token::Kind::Character, "\r", 1, mLine, bColumn);
			case 't':
				return new Token(Token::Kind::Character, "\t", 1, mLine, bColumn);
			case '\'':
				return new Token(Token::Kind::Character, "\'", 1, mLine, bColumn);
			}
		}
		if (Get() == '\'')
			return new Token(Token::Kind::Character, beg, 1, mLine, bColumn);

		return new Token(Token::Kind::Unknown, beg, mCurrent - 1, mLine, bColumn);
	}

	Token* Lexer::Comment()
	{
		const char* beg = mCurrent;
		int bLine = mLine, bColumn = mColumn;
		Get();
		if (Peek() == '\\')
		{
			Get();
			while (!(CheckNewLine(Peek()) || Peek() == 0))
			{
				Get();
			}
			Get();
			return new Token(Token::Kind::Comment, beg + 2, mCurrent - 1, bLine, bColumn);
		}
		else if (Peek() == '*')
		{
			Get();
			while (Peek() != 0)
			{
				if (Get() == '*' && Get() == '\\')
					return new Token(Token::Kind::Comment, beg + 2, mCurrent - 2, bLine, bColumn);
			}
			return new Token(Token::Kind::Comment, beg + 2, mCurrent - 1, bLine, bColumn);
		}

		return new Token(Token::Kind::Unknown, beg, mCurrent, bLine, bColumn);
	}

	Token* Lexer::Number()
	{
		const char* beg = mCurrent;
		int bColumn = mColumn;
		Get();
		while (IsDigit(Peek()))
			Get();
		return new Token(Token::Kind::Number, beg, mCurrent, mLine, bColumn);
	}

	Token* Lexer::String()
	{
		Get();
		const char* beg = mCurrent;
		int bLine = mLine, bColumn = mColumn;
		while (Peek() != '"' || Peek(-1) == '\\')
			Get();
		Get();
		return new Token(Token::Kind::String, beg, mCurrent - 1, bLine, bColumn);
	}

	Token* Lexer::Equal()
	{
		const char* beg = mCurrent;
		int bLine = mLine, bColumn = mColumn;

		Get();
		if (Peek() == '=')
		{
			Get();
			switch (*beg)
			{
			case '=':
				return new Token(Token::Kind::EqualEqual, beg, mCurrent, bLine, bColumn);
			case '!':
				return new Token(Token::Kind::ExclamationEqual, beg, mCurrent, bLine, bColumn);
			case '+':
				return new Token(Token::Kind::PlusEqual, beg, mCurrent, bLine, bColumn);
			case '-':
				return new Token(Token::Kind::MinusEqual, beg, mCurrent, bLine, bColumn);
			case '*':
				return new Token(Token::Kind::AsteriskEqual, beg, mCurrent, bLine, bColumn);
			case '/':
				return new Token(Token::Kind::SlashEqual, beg, mCurrent, bLine, bColumn);
			case '%':
				return new Token(Token::Kind::PercentageEqual, beg, mCurrent, bLine, bColumn);
			case '<':
				return new Token(Token::Kind::LeftArrowEqual, beg, mCurrent, bLine, bColumn);
			case '>':
				return new Token(Token::Kind::RightArrowEqual, beg, mCurrent, bLine, bColumn);
			}
		}
		if (Peek() == *beg)
		{
			Get();
			switch (*beg)
			{
			case '+':
				return new Token(Token::Kind::PlusPlus, beg, mCurrent, bLine, bColumn);
			case '-':
				return new Token(Token::Kind::MinusMinus, beg, mCurrent, bLine, bColumn);
			case '<':
				return new Token(Token::Kind::LeftArrowArrow, beg, mCurrent, bLine, bColumn);
			case '>':
				return new Token(Token::Kind::RightArrowArrow, beg, mCurrent, bLine, bColumn);
			}
		}

		switch (*beg)
		{
		case '=':
			return new Token(Token::Kind::Equal, beg, mCurrent, bLine, bColumn);
		case '!':
			return new Token(Token::Kind::Exclamation, beg, mCurrent, bLine, bColumn);
		case '+':
			return new Token(Token::Kind::Plus, beg, mCurrent, bLine, bColumn);
		case '-':
			return new Token(Token::Kind::Minus, beg, mCurrent, bLine, bColumn);
		case '*':
			return new Token(Token::Kind::Asterisk, beg, mCurrent, bLine, bColumn);
		case '/':
			return new Token(Token::Kind::Slash, beg, mCurrent, bLine, bColumn);
		case '%':
			return new Token(Token::Kind::Percentage, beg, mCurrent, bLine, bColumn);
		case '<':
			return new Token(Token::Kind::LeftArrowEqual, beg, mCurrent, bLine, bColumn);
		case '>':
			return new Token(Token::Kind::RightArrowEqual, beg, mCurrent, bLine, bColumn);
		}

		return new Token(Token::Kind::Unknown, beg, mCurrent, bLine, bColumn);
	}
}