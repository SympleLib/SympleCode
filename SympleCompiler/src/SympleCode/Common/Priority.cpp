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
		case Token::Kind::PlusPlus:
		case Token::Kind::MinusMinus:
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
		case Token::Kind::EqualEqual:
		case Token::Kind::ExclamationEqual:
		case Token::Kind::LeftArrow:
		case Token::Kind::RightArrow:
			return 1;
		case Token::Kind::Plus:
		case Token::Kind::Minus:
			return 2;
		case Token::Kind::Asterisk:
		case Token::Kind::Slash:
		case Token::Kind::Percentage:
			return 3;
		case Token::Kind::OpenBracket:
			return 4;
		}

		return -1;
	}

	int Priority::AssignmentOperatorPriority(const Token* token)
	{
		switch (token->GetKind())
		{
		case Token::Kind::Equal:
		case Token::Kind::PlusEqual:
		case Token::Kind::MinusEqual:
		case Token::Kind::AsteriskEqual:
		case Token::Kind::SlashEqual:
		case Token::Kind::PercentageEqual:
			return 0;
		}

		return -1;
	}
}