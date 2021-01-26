#pragma once

#include "SympleCode/Syntax/Token.h"

namespace Symple::Syntax
{
	static struct Facts
	{
		static unsigned GetUnaryOperatorPrecedence(Token::Kind);
		static unsigned GetBinaryOperatorPrecedence(Token::Kind);
	};
}