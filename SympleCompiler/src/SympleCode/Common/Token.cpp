#include "SympleCode/Common/Token.h"

namespace Symple
{
	Token::Token(Kind kind, int line, int column)
		: mKind(kind), mLine(line), mColumn(column) {}

	Token::Token(Kind kind, const char* beg, size_t len, int line, int column)
		: mKind(kind), mLex(beg, len), mLine(line), mColumn(column) {}

	Token::Token(Kind kind, const char* beg, const char* end, int line, int column)
		: mKind(kind), mLex(beg, std::distance(beg, end)), mLine(line), mColumn(column) {}

	const Token* const Token::Default = new Token;

	bool Token::Is(Kind kind) const
	{
		return mKind == kind;
	}

	bool Token::IsEither(std::initializer_list<Kind> kinds) const
	{
		for (Kind kind : kinds)
			if (mKind == kind)
				return true;
		return false;
	}

	const Token::Kind& Token::GetKind() const
	{
		return mKind;
	}

	const std::string_view& Token::GetLex() const
	{
		return mLex;
	}

	int Token::GetLine() const
	{
		return mLine;
	}

	int Token::GetColumn() const
	{
		return mColumn;
	}

	const char* Token::KindString(Kind kind)
	{
		switch (kind)
		{
		case Kind::Unknown:
			return "Unknown";
		case Kind::EndOfFile:
			return "End Of File";
		default:
			return KindMap[(int)kind];
		}
	}
}