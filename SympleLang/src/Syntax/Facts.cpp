#include "SympleCode/Syntax/Facts.h"

namespace Symple::Syntax
{
	unsigned Facts::GetUnaryOperatorPrecedence(Token::Kind kind)
	{
		switch (kind)
		{
		case Token::Plus:
		case Token::Dash:
			return 3;

		default:
			return 0;
		}
	}

	unsigned Facts::GetBinaryOperatorPrecedence(Token::Kind kind)
	{
		switch (kind)
		{
		case Token::Asterisk:
		case Token::Slash:
			return 2;

		case Token::Plus:
		case Token::Dash:
			return 1;

		default:
			return 0;
		}
	}
}