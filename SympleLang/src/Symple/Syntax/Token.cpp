#include "Symple/Syntax/Token.h"

namespace Symple
{
	Token::Token(TokenKind kind)
		: kind(kind) {}


	TokenKind Token::GetKind() const
	{ return kind; }

	std::string_view Token::GetText() const
	{ return text; }
}