#pragma once

#include "SympleCode/Syntax/CallExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

#include "SympleCode/Symbol/FunctionSymbol.h"

namespace Symple::Binding
{
	typedef Promise<std::pair<shared_ptr<Symbol::FunctionSymbol>, ExpressionList>, shared_ptr<Syntax::CallExpressionSyntax>> FunctionPromise;

	class BoundCallExpression : public BoundExpression
	{
	private:
		shared_ptr<FunctionPromise> mFunction;
	public:
		BoundCallExpression(shared_ptr<Syntax::CallExpressionSyntax> syntax, shared_ptr<FunctionPromise> func)
			: BoundExpression(syntax), mFunction(func) {}

		virtual Kind GetKind() override
		{ return CallExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return GetFunction() ? GetFunction()->GetType() : Symbol::TypeSymbol::IntType; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			//os << " '" << GetSyntax()->GetToken()->GetText(); os << '(' << (GetArguments().size()) << ")'";
			
			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto arg : GetArguments())
			{ os.put('\n'); arg->Print(os, newIndent, arg == GetArguments().back()); }
		}

		shared_ptr<Symbol::FunctionSymbol> GetFunction()
		{ return mFunction->GetObject().first; }

		ExpressionList GetArguments()
		{ return mFunction->GetObject().second; }
	};
}