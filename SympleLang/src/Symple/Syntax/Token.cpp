#include "Symple/Common/Common.h"
#include "Symple/Syntax/Token.h"

namespace Symple
{
	Token::Token(TokenKind kind, std::string_view text)
		: kind(kind), text(text) {}

	TokenKind Token::GetKind() const
	{ return kind; }

	std::string_view Token::GetText() const
	{ return text; }
}