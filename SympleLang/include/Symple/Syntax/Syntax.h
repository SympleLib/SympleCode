#pragma once

#include "Symple/Syntax/Token.h"

namespace Symple
{
	enum class SY_API SyntaxKind: uint32
	{
		Unknown,

		Expression,
		LiteralExpression,
	};

	class SY_API Syntax
	{
	public:
		virtual SyntaxKind GetKind() const
		{ return SyntaxKind::Unknown; }

		virtual GlobalRef<Token> GetToken() const = 0;

		SY_PROPERTY_GET(GetKind) SyntaxKind Kind;
		SY_PROPERTY_GET(GetToken) GlobalRef<Token> MainToken;
	};
}