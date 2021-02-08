#pragma once

#include "SympleCode/Syntax/VariableDeclarationSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"
#include "SympleCode/Binding/BoundExpression.h"
#include "SympleCode/Symbol/VariableSymbol.h"

namespace Symple::Binding
{
	class BoundVariableDeclaration : public BoundStatement
	{
	private:
		shared_ptr<Symbol::VariableSymbol> mSymbol;
		shared_ptr<BoundExpression> mInitializer;
	public:
		BoundVariableDeclaration(shared_ptr<Syntax::VariableDeclarationSyntax> syntax, shared_ptr<Symbol::VariableSymbol> symbol, shared_ptr<BoundExpression> init)
			: BoundStatement(syntax), mSymbol(symbol), mInitializer(init) {}

		virtual Kind GetKind() override
		{ return VariableDeclaration; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetSymbol()->Print(os, newIndent, !GetInitializer(), "Symbol = ");
			if (GetInitializer())
			{ os.put('\n'); GetInitializer()->Print(os, newIndent, true, "Initializer = "); }
		}

		shared_ptr<Symbol::VariableSymbol> GetSymbol()
		{ return mSymbol; }

		shared_ptr<BoundExpression> GetInitializer()
		{ return mInitializer; }
	};
}