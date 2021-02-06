#pragma once

#include "SympleCode/Syntax/NameExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

#include "SympleCode/Symbol/VariableSymbol.h"

namespace Symple::Binding
{
	class BoundVariableExpression : public BoundExpression
	{
	private:
		shared_ptr<Symbol::VariableSymbol> mSymbol;
	public:
		BoundVariableExpression(shared_ptr<Syntax::NameExpressionSyntax> syntax, shared_ptr<Symbol::VariableSymbol> var)
			: BoundExpression(syntax), mSymbol(var)
		{}

		virtual Kind GetKind() override
		{ return VariableExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return mSymbol->GetType(); }

		virtual bool IsMutable() override
		{ return true; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetSymbol()->Print(os, newIndent, true, "Symbol = ");
		}

		shared_ptr<Symbol::VariableSymbol> GetSymbol()
		{ return mSymbol; }
	};
}