#pragma once

#include "Symple/Code/Type/Type.h"
#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	struct SYC_API Precedence
	{
		Precedence() = delete;

		static uint32 Unary(TokenKind);
		static uint32 Binary(TokenKind);
	};

	struct SYC_API TokenFacts
	{
		TokenFacts() = delete;

		static TypeKind ToType(TokenKind);

		static bool IsTypeBase(TokenKind);
		static bool IsTypePointer(TokenKind);
		static bool IsTypeRef(TokenKind);
	};
}