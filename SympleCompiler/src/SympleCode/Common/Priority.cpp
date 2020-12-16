#include "SympleCode/Common/Priority.h"

namespace Symple
{
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
		}

		return -1;
	}
}