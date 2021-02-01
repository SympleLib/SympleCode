#pragma once

#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundImplicitCastExpression : public BoundExpression
	{
	private:
		shared_ptr<Type> mType;
		shared_ptr<BoundExpression> mCastee;
	public:
		BoundImplicitCastExpression(shared_ptr<Syntax::ExpressionSyntax> syntax, shared_ptr<Type> type, shared_ptr<BoundExpression> castee)
			: BoundExpression(syntax), mType(type), mCastee(castee) {}

		virtual Kind GetKind() override
		{ return ImplicitCastExpression; }

		virtual shared_ptr<Type> GetType() override
		{ return mType; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetType()->Print(os, newIndent, false, "Type = ");
			os.put('\n'); GetCastee()->Print(os, newIndent, true, "Castee = ");
		}

		shared_ptr<BoundExpression> GetCastee()
		{ return mCastee; }

		shared_ptr<Type> GetCasteeType()
		{ return GetCastee()->GetType(); }
	};
}