#pragma once

#include "Symple/Syntax/Syntax.h"

namespace Symple
{
	class SY_API ExpressionSyntax: public Syntax
	{
	public:
		virtual SyntaxKind GetKind() const override
		{ return SyntaxKind::Expression; }
	};
}