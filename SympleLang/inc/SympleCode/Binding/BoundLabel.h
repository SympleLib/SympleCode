#pragma once

#include "SympleCode/Syntax/LabelSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"

#include "SympleCode/Symbol/LabelSymbol.h"

namespace Symple::Binding
{
	class BoundLabel : public BoundStatement
	{
	private:
		shared_ptr<Symbol::LabelSymbol> mSymbol;
	public:
		BoundLabel(shared_ptr<Syntax::LabelSyntax> syntax, shared_ptr<Symbol::LabelSymbol> symbol)
			: BoundStatement(syntax), mSymbol(symbol)
		{}

		virtual Kind GetKind() override
		{ return Label; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " \'" << GetLabel() << '\'';
		}

		shared_ptr<Symbol::LabelSymbol> GetSymbol()
		{ return mSymbol; }

		std::string_view GetLabel()
		{ return GetSymbol()->GetLabel(); }
	};
}