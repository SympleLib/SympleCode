#pragma once

#include "SympleCode/Syntax/LiteralExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundDefaultExpression : public BoundExpression
	{
	public:
		BoundDefaultExpression(shared_ptr<Syntax::LiteralExpressionSyntax> syntax)
			: BoundExpression(syntax) {}

		virtual Kind GetKind() override
		{ return DefaultExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return Symbol::TypeSymbol::VoidType; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
		}
	};
}