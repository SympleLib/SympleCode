#pragma once

#include "SympleCode/Syntax/LiteralExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundLiteralExpression : public BoundExpression
	{
	private:
		shared_ptr<Symbol::TypeSymbol> mType;
		shared_ptr<BoundConstant> mValue;
	public:
		BoundLiteralExpression(shared_ptr<Syntax::LiteralExpressionSyntax> syntax, shared_ptr<Symbol::TypeSymbol> ty, shared_ptr<BoundConstant> val)
			: BoundExpression(syntax), mType(ty), mValue(val)
		{}

		virtual Kind GetKind() override
		{ return LiteralExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return mType; }

		virtual shared_ptr<BoundConstant> ConstantValue() override
		{ return mValue; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			GetToken()->Print(os, indent, last, std::string(label)); os << " (BoundLiteralExpression)";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetType()->Print(os, newIndent, true, "Type = ");
		}

		shared_ptr<Syntax::Token> GetToken()
		{ return GetSyntax()->GetToken(); }
	};
}