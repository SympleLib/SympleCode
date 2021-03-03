#pragma once

#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	struct Precedence
	{
		Precedence() = delete;

		static uint32 Unary(TokenKind);
		static uint32 Binary(TokenKind);
	};
}