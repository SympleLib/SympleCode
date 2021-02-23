#pragma once

#include "SympleCode/Syntax/GotoStatementSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"

#include "SympleCode/Symbol/Promise.h"
#include "SympleCode/Symbol/LabelSymbol.h"

namespace Symple::Binding
{
	typedef Promise<shared_ptr<Symbol::LabelSymbol>, shared_ptr<Syntax::GotoStatementSyntax>> GotoPromise;

	class BoundGotoStatement : public BoundStatement
	{
	private:
		shared_ptr<GotoPromise> mLabel;
	public:
		BoundGotoStatement(shared_ptr<Syntax::GotoStatementSyntax> syntax, shared_ptr<GotoPromise> symbol)
			: BoundStatement(syntax), mLabel(symbol)
		{}

		virtual Kind GetKind() override
		{ return GotoStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " \'" << GetLabel() << '\'';
		}

		auto GetLabelSymbol()
		{ return mLabel; }

		std::string_view GetLabel()
		{ return mLabel->GetPrompt()->GetToken()->GetText(); }
	};
}