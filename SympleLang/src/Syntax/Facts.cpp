#include "SympleCode/Syntax/Facts.h"

namespace Symple::Syntax
{
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