#include "SympleCode/Common/Token.h"

namespace Symple
{
	Token::Token(Kind kind)
		: mKind(kind) {}

	Token::Token(Kind kind, const char* beg, size_t len)
		: mKind(kind), mLex(beg, len) {}

	Token::Token(Kind kind, const char* beg, const char* end)
		: mKind(kind), mLex(beg, std::distance(beg, end)) {}

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

	constexpr const char* Token::KindString(Kind kind)
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