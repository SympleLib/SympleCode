#include "SympleCode/Parser/Lexer.h"

#include <sstream>

namespace Symple
{
	Lexer::Lexer(const char* source, const char* file)
		: mSource(source), mCurrent(source), mFile(file), mLine(1), mColumn(0) {}

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
		case ':':
			return Atom(Token::Kind::Colon);
		case '@':
			return Atom(Token::Kind::At);
		case '?':
			return Atom(Token::Kind::QuestionMark);
		case '|':
			return Equal();
		case '&':
			return Equal();
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
		return new Token(kind, mCurrent++, 1, mFile, mLine, mColumn);
	}

	Token* Lexer::Identifier()
	{
		const char* beg = mCurrent;
		int bColumn = mColumn;
		Get();
		while (IsIdentifier(Peek()))
			Get();
		std::string_view identifier(beg, std::distance(beg, mCurrent));
		if (identifier == "asm")
			return new Token(Token::Kind::Asm, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "null")
			return new Token(Token::Kind::Null, beg, mCurrent, mFile, mLine, bColumn);

		if (identifier == "auto" || identifier == "var")
			return new Token(Token::Kind::Auto, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "true")
			return new Token(Token::Kind::True, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "false")
			return new Token(Token::Kind::False, beg, mCurrent, mFile, mLine, bColumn);

		if (identifier == "return")
			return new Token(Token::Kind::Return, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "break")
			return new Token(Token::Kind::Break, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "while")
			return new Token(Token::Kind::While, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "for")
			return new Token(Token::Kind::For, beg, mCurrent, mFile, mLine, bColumn);

		if (identifier == "hint")
			return new Token(Token::Kind::Hint, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "extern")
			return new Token(Token::Kind::Extern, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "shared")
			return new Token(Token::Kind::Shared, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "static")
			return new Token(Token::Kind::Static, beg, mCurrent, mFile, mLine, bColumn);

		if (identifier == "if")
			return new Token(Token::Kind::If, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "else")
			return new Token(Token::Kind::Else, beg, mCurrent, mFile, mLine, bColumn);

		if (identifier == "symplecall")
			return new Token(Token::Kind::SympleCall, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "stdcall")
			return new Token(Token::Kind::StdCall, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "ccall")
			return new Token(Token::Kind::CCall, beg, mCurrent, mFile, mLine, bColumn);

		if (identifier == "mut" || identifier == "mutable")
			return new Token(Token::Kind::Mutable, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "priv" || identifier == "private")
			return new Token(Token::Kind::Private, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "unsigned")
			return new Token(Token::Kind::Unsigned, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "signed")
			return new Token(Token::Kind::Signed, beg, mCurrent, mFile, mLine, bColumn);

		if (identifier == "struct")
			return new Token(Token::Kind::Struct, beg, mCurrent, mFile, mLine, bColumn);
		if (identifier == "enum")
			return new Token(Token::Kind::Enum, beg, mCurrent, mFile, mLine, bColumn);
		return new Token(Token::Kind::Identifier, beg, mCurrent, mFile, mLine, bColumn);
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
		return new Token(Token::Kind::Preprocess, beg, mCurrent - 1, mFile, bLine, bColumn);
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
				return new Token(Token::Kind::Unknown, beg, mCurrent - 1, mFile, mLine, bColumn);
			switch (mc)
			{
			case 'n':
				return new Token(Token::Kind::Character, "\n", 1, mFile, mLine, bColumn);
			case 'r':
				return new Token(Token::Kind::Character, "\r", 1, mFile, mLine, bColumn);
			case 't':
				return new Token(Token::Kind::Character, "\t", 1, mFile, mLine, bColumn);
			case '\'':
				return new Token(Token::Kind::Character, "\'", 1, mFile, mLine, bColumn);
			case '\\':
				return new Token(Token::Kind::Character, "\\", 1, mFile, mLine, bColumn);
			}
		}
		if (Get() == '\'')
			return new Token(Token::Kind::Character, beg, 1, mFile, mLine, bColumn);

		return new Token(Token::Kind::Unknown, beg, mCurrent - 1, mFile, mLine, bColumn);
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
			return new Token(Token::Kind::Comment, beg + 2, mCurrent - 1, mFile, bLine, bColumn);
		}
		else if (Peek() == '*')
		{
			Get();
			while (Peek() != 0)
			{
				if (Get() == '*' && Get() == '\\')
					return new Token(Token::Kind::Comment, beg + 2, mCurrent - 2, mFile, bLine, bColumn);
			}
			return new Token(Token::Kind::Comment, beg + 2, mCurrent - 1, mFile, bLine, bColumn);
		}

		return new Token(Token::Kind::Unknown, beg, mCurrent, mFile, bLine, bColumn);
	}

	Token* Lexer::Number()
	{
		const char* beg = mCurrent;
		int bColumn = mColumn;
		Get();
		while (IsDigit(Peek()))
			Get();
		return new Token(Token::Kind::Number, beg, mCurrent, mFile, mLine, bColumn);
	}

	Token* Lexer::String()
	{
		Get();
		const char* beg = mCurrent;
		int bLine = mLine, bColumn = mColumn;

		std::stringstream ss;
		while (Peek() != '"')
		{
			char c = Get();
			if (c == '\\')
			{
				char c = Get();

				switch (c)
				{
				case 'n':
					ss << '\n';
					break;
				case 'r':
					ss << '\r';
					break;
				case 't':
					ss << '\t';
					break;
				case '\"':
					ss << '\"';
					break;
				case '\\':
					ss << '\\';
					break;
				default:
					return new Token(Token::Kind::Unknown, beg, mCurrent, mFile, bLine, bColumn);
				}
			}
			else
				ss << c;
		}
		Get();

		std::string* str = new std::string(ss.str());
		return new Token(Token::Kind::String, str->c_str(), str->length(), mFile, bLine, bColumn);
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
				return new Token(Token::Kind::EqualEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '!':
				return new Token(Token::Kind::ExclamationEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '+':
				return new Token(Token::Kind::PlusEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '-':
				return new Token(Token::Kind::MinusEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '*':
				return new Token(Token::Kind::AsteriskEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '/':
				return new Token(Token::Kind::SlashEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '%':
				return new Token(Token::Kind::PercentageEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '<':
				return new Token(Token::Kind::LeftArrowEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '>':
				return new Token(Token::Kind::RightArrowEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '|':
				return new Token(Token::Kind::PipeEqual, beg, mCurrent, mFile, bLine, bColumn);
			case '&':
				return new Token(Token::Kind::AmpersandEqual, beg, mCurrent, mFile, bLine, bColumn);
			}
		}
		if (Peek() == *beg)
		{
			switch (*beg)
			{
			case '+':
				Get();
				return new Token(Token::Kind::PlusPlus, beg, mCurrent, mFile, bLine, bColumn);
			case '-':
				Get();
				return new Token(Token::Kind::MinusMinus, beg, mCurrent, mFile, bLine, bColumn);
			case '<':
				Get();
				return new Token(Token::Kind::LeftArrowArrow, beg, mCurrent, mFile, bLine, bColumn);
			case '>':
				Get();
				return new Token(Token::Kind::RightArrowArrow, beg, mCurrent, mFile, bLine, bColumn);
			case '|':
				Get();
				return new Token(Token::Kind::PipePipe, beg, mCurrent, mFile, bLine, bColumn);
			case '&':
				Get();
				return new Token(Token::Kind::AmpersandAmpersand, beg, mCurrent, mFile, bLine, bColumn);
			}
		}

		switch (*beg)
		{
		case '=':
			return new Token(Token::Kind::Equal, beg, mCurrent, mFile, bLine, bColumn);
		case '!':
			return new Token(Token::Kind::Exclamation, beg, mCurrent, mFile, bLine, bColumn);
		case '+':
			return new Token(Token::Kind::Plus, beg, mCurrent, mFile, bLine, bColumn);
		case '-':
			return new Token(Token::Kind::Minus, beg, mCurrent, mFile, bLine, bColumn);
		case '*':
			return new Token(Token::Kind::Asterisk, beg, mCurrent, mFile, bLine, bColumn);
		case '/':
			return new Token(Token::Kind::Slash, beg, mCurrent, mFile, bLine, bColumn);
		case '%':
			return new Token(Token::Kind::Percentage, beg, mCurrent, mFile, bLine, bColumn);
		case '<':
			return new Token(Token::Kind::LeftArrowEqual, beg, mCurrent, mFile, bLine, bColumn);
		case '>':
			return new Token(Token::Kind::RightArrowEqual, beg, mCurrent, mFile, bLine, bColumn);
		case '|':
			return new Token(Token::Kind::Pipe, beg, mCurrent, mFile, bLine, bColumn);
		case '&':
			return new Token(Token::Kind::Ampersand, beg, mCurrent, mFile, bLine, bColumn);
		}

		return new Token(Token::Kind::Unknown, beg, mCurrent, mFile, bLine, bColumn);
	}
}