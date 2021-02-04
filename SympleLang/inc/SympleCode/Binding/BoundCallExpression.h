#pragma once

#include "SympleCode/Syntax/CallExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

#include "SympleCode/Symbol/FunctionSymbol.h"

namespace Symple::Binding
{
	class BoundCallExpression : public BoundExpression
	{
	private:
		shared_ptr<Symbol::FunctionSymbol> mFunction;
		ExpressionList mArguments;
	public:
		BoundCallExpression(shared_ptr<Syntax::CallExpressionSyntax> syntax, shared_ptr<Symbol::FunctionSymbol> func, ExpressionList args)
			: BoundExpression(syntax), mFunction(func), mArguments(args) {}

		virtual Kind GetKind() override
		{ return CallExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return GetFunction()->GetType(); }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '" << GetFunction()->GetName(); os << '(' << GetArguments().size() << ")'";
			
			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto arg : GetArguments())
			{ os.put('\n'); arg->Print(os, newIndent, arg == GetArguments().back()); }
		}

		shared_ptr<Symbol::FunctionSymbol> GetFunction()
		{ return mFunction; }

		ExpressionList GetArguments()
		{ return mArguments; }
	};
}