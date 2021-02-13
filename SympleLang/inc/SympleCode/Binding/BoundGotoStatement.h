#pragma once

#include "SympleCode/Syntax/GotoStatementSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"

#include "SympleCode/Symbol/LabelSymbol.h"

namespace Symple::Binding
{
	class BoundGotoStatement : public BoundStatement
	{
	private:
		//shared_ptr<Symbol::LabelSymbol> mLabel;
	public:
		BoundGotoStatement(shared_ptr<Syntax::GotoStatementSyntax> syntax/*, shared_ptr<Symbol::LabelSymbol> symbol*/)
			: BoundStatement(syntax)/*, mLabel(symbol)*/
		{}

		virtual Kind GetKind() override
		{ return GotoStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " \'" << GetLabel() << '\'';
		}

		//shared_ptr<Symbol::LabelSymbol> GetLabelSymbol()
		//{ return mLabel; }

		std::string_view GetLabel()
		{ return GetSyntax()->GetToken()->GetText(); }
	};
}