#include "SympleCode/Common/Priority.h"

namespace Symple
{
	int Priority::UnaryOperatorPriority(const Token* token)
	{
		switch (token->GetKind())
		{
		case Token::Kind::Asterisk:
			return 1;
		case Token::Kind::Minus:
			return 3;
		case Token::Kind::At:
			return 4;
		}

		return -1;
	}

	int Priority::BinaryOperatorPriority(const Token* token)
	{
		switch (token->GetKind())
		{
		case Token::Kind::Equal:
			return 0;
		case Token::Kind::EqualEqual:
		case Token::Kind::NotEqual:
			return 1;
		case Token::Kind::Plus:
		case Token::Kind::Minus:
			return 2;
		case Token::Kind::Asterisk:
		//case Token::Kind::Slash:
			return 3;
		case Token::Kind::OpenBracket:
			return 4;
		}

		return -1;
	}
}