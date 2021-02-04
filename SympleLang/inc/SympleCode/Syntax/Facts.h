#pragma once

#include "SympleCode/Syntax/Token.h"

namespace Symple::Syntax
{
	struct Facts
	{
		static unsigned GetUnaryOperatorPrecedence(Token::Kind);
		static unsigned GetBinaryOperatorPrecedence(Token::Kind);
	};
}