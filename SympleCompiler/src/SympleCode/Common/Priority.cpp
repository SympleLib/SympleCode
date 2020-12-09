#include "SympleCode/Common/Priority.h"

namespace Symple
{
	int Priority::BinaryOperatorPriority(const Token* token)
	{
		switch (token->GetKind())
		{
		case Token::Kind::Plus:
		case Token::Kind::Minus:
			return 0;
		case Token::Kind::Asterisk:
		case Token::Kind::Slash:
			return 1;
		}

		return -1;
	}
}