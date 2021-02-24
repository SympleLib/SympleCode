#pragma once

#include "SympleCode/Syntax/BinaryExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

#include "SympleCode/Symbol/Promise.h"
#include "SympleCode/Symbol/MemberSymbol.h"

namespace Symple::Binding
{
	typedef Promise<shared_ptr<Symbol::MemberSymbol>, std::pair<shared_ptr<BoundExpression>, shared_ptr<Syntax::BinaryExpressionSyntax>>> MemberPromise;

	class BoundFieldExpression: public BoundExpression
	{
	private:
		shared_ptr<BoundExpression> mOperand;
		shared_ptr<MemberPromise> mMember;
	public:
		BoundFieldExpression(shared_ptr<Syntax::BinaryExpressionSyntax> syntax, shared_ptr<BoundExpression> operand, shared_ptr<MemberPromise> member)
			: BoundExpression(syntax), mOperand(operand), mMember(member) {}

		virtual Kind GetKind() override
		{ return FieldExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return GetMember() ? GetMember()->GetType() : Symbol::TypeSymbol::VoidType; }

		virtual bool IsMutable() override
		{ return true; }

		virtual void Print(std::ostream &os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os << '\n'; GetOperand()->Print(os, newIndent, false, "Operand = ");

			if (GetMember())
			{ os << '\n'; GetMember()->Print(os, newIndent, true, "Member = "); }
		}

		shared_ptr<BoundExpression> GetOperand()
		{ return mOperand; }

		shared_ptr<Symbol::MemberSymbol> GetMember()
		{ return mMember->GetObject(); }
	};
}