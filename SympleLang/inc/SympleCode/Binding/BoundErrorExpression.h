#pragma once

#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundErrorExpression : public BoundExpression
	{
	public:
		BoundErrorExpression(shared_ptr<Syntax::ExpressionSyntax> syntax)
			: BoundExpression(syntax) {}

		virtual Kind GetKind() override
		{ return ErrorExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return Symbol::TypeSymbol::ErrorType; }
	};
}