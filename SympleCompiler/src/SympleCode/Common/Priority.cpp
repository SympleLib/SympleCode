#include "SympleCode/Common/Priority.h"

namespace Symple
{
	int Priority::UnaryOperatorPriority(const Token* token)
	{
		switch (token->GetKind())
		{
		case Token::Kind::Exclamation:
			return 0;
		case Token::Kind::Minus:
			return 2;
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
		case Token::Kind::LeftArrowEqual:
		case Token::Kind::RightArrowEqual:
			return 0;
		case Token::Kind::LeftArrowArrow:
		case Token::Kind::RightArrowArrow:
		case Token::Kind::Pipe:
		case Token::Kind::PipePipe:
		case Token::Kind::Ampersand:
		case Token::Kind::AmpersandAmpersand:
			return 1;
		case Token::Kind::Plus:
		case Token::Kind::Minus:
			return 2;
		case Token::Kind::Asterisk:
		case Token::Kind::Slash:
		case Token::Kind::Percentage:
			return 3;
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
		case Token::Kind::PipeEqual:
		case Token::Kind::AmpersandEqual:
			return 0;
		}

		return -1;
	}
}