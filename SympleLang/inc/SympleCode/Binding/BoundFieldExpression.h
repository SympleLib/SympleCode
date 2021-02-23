#pragma once

#include "SympleCode/Syntax/BinaryExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"

#include "SympleCode/Symbol/Promise.h"
#include "SympleCode/Symbol/MemberSymbol.h"

namespace Symple::Binding
{
	typedef Promise<shared_ptr<Symbol::MemberSymbol>, shared_ptr<Syntax::BinaryExpressionSyntax>> MemberPromise;

	class BoundFieldExpression: public BoundExpression
	{
	private:
		shared_ptr<MemberPromise> mMember;
	public:
		BoundFieldExpression(shared_ptr<Syntax::BinaryExpressionSyntax> syntax, shared_ptr<MemberPromise> member)
			: BoundExpression(syntax), mMember(member) {}

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

			if (GetMember())
			{ os.put('\n'); GetMember()->Print(os, newIndent, true, "Member = "); }
		}

		shared_ptr<Symbol::MemberSymbol> GetMember()
		{ return mMember->GetObject(); }
	};
}