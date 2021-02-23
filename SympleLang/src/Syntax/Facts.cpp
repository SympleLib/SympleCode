#include "SympleCode/Syntax/Facts.h"

namespace Symple::Syntax
{
	unsigned Facts::GetUnaryOperatorPrecedence(Token::Kind kind)
	{
		switch (kind)
		{
		case Token::Exclamation:
		case Token::Plus:
		case Token::Dash:
			return 2;

		default:
			return 0;
		}
	}

	unsigned Facts::GetBinaryOperatorPrecedence(Token::Kind kind)
	{
		switch (kind)
		{
		case Token::Period:
			return 1;

		case Token::Percentage:
		case Token::Asterisk:
		case Token::Slash:
			return 3;

		case Token::Plus:
		case Token::Dash:
			return 4;

		case Token::Equal:
			return 14;

		default:
			return 0;
		}
	}
}