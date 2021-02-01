#pragma once

#include "SympleCode/Syntax/LiteralExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundLiteralExpression : public BoundExpression
	{
	private:
		shared_ptr<Type> mType;
	public:
		BoundLiteralExpression(shared_ptr<Syntax::LiteralExpressionSyntax> syntax, shared_ptr<Type> ty)
			: BoundExpression(syntax), mType(ty)
		{}

		virtual Kind GetKind() override
		{ return LiteralExpression; }

		virtual shared_ptr<Type> GetType() override
		{ return mType; }

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