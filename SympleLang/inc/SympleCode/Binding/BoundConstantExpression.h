#pragma once

#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundConstantExpression : public BoundExpression
	{
	private:
		shared_ptr<BoundConstant> mValue;
	public:
		BoundConstantExpression(shared_ptr<BoundConstant> val)
			: BoundExpression(make_shared<Syntax::ExpressionSyntax>(Syntax::Token::Default)), mValue(val) {}

		virtual Kind GetKind() override
		{ return ConstantExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return Symbol::TypeSymbol::LongType; }

		virtual shared_ptr<BoundConstant> ConstantValue() override
		{ return mValue; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			ConstantValue()->Print(os, newIndent, true, "Constant = ");
		}
	};
}