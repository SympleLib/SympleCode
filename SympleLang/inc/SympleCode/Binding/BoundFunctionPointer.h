#pragma once

#include "SympleCode/Syntax/NameExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

#include "SympleCode/Symbol/FunctionSymbol.h"

namespace Symple::Binding
{
	class BoundFunctionPointer : public BoundExpression
	{
	private:
		shared_ptr<Symbol::FunctionSymbol> mSymbol;
	public:
		BoundFunctionPointer(shared_ptr<Syntax::NameExpressionSyntax> syntax, shared_ptr<Symbol::FunctionSymbol> fn)
			: BoundExpression(syntax), mSymbol(fn)
		{}

		virtual Kind GetKind() override
		{ return FunctionPointer; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return Symbol::TypeSymbol::VoidPointerType; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetSymbol()->Print(os, newIndent, true, "Symbol = ");
		}

		shared_ptr<Symbol::FunctionSymbol> GetSymbol()
		{ return mSymbol; }
	};
}