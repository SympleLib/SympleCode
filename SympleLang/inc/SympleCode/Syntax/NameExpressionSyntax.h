#pragma once

#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class NameExpressionSyntax : public ExpressionSyntax
	{
	public:
		NameExpressionSyntax(shared_ptr<Token> tok)
			: ExpressionSyntax(tok) {}

		virtual Kind GetKind() override
		{ return NameExpression; }
	};
}