#include "Symple/Code/Parse/Facts.h"

namespace Symple::Code
{
	uint32 Precedence::Unary(TokenKind kind)
	{
		switch (kind)
		{
		default:
			return 0;
		case TokenKind::Plus:
		case TokenKind::Minus:
			return 5;
		case TokenKind::Star:
		case TokenKind::At:
			return 6;
		}
	}

	uint32 Precedence::Binary(TokenKind kind)
	{
		switch (kind)
		{
		default:
			return 0;
		case TokenKind::Equal:
			return 1;
		case TokenKind::Plus:
		case TokenKind::Minus:
			return 2;
		case TokenKind::Star:
		case TokenKind::Slash:
		case TokenKind::Percent:
			return 3;
		case TokenKind::At:
		case TokenKind::CarotCarot:
			return 4;
		case TokenKind::Arrow:
			return 7;
		}
	}
}