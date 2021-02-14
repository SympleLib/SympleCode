#pragma once

#include "SympleCode/Syntax/IfStatementSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"
#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundIfStatement : public BoundStatement
	{
	private:
		shared_ptr<BoundExpression> mCondition;
		shared_ptr<BoundStatement> mThen;
		shared_ptr<BoundStatement> mElse;
	public:
		BoundIfStatement(shared_ptr<Syntax::IfStatementSyntax> syntax, shared_ptr<BoundExpression> cond, shared_ptr<BoundStatement> then, shared_ptr<BoundStatement> elze)
			: BoundStatement(syntax), mCondition(cond), mThen(then), mElse(elze) {}

		virtual Kind GetKind() override
		{ return IfStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetCondition()->Print(os, newIndent, false, "Condition = ");
			os.put('\n'); GetThen()->Print(os, newIndent, false, "Then = ");
			os.put('\n'); GetElse()->Print(os, newIndent, true, "Else = ");
		}

		shared_ptr<BoundExpression> GetCondition()
		{ return mCondition; }

		shared_ptr<BoundStatement> GetThen()
		{ return mThen; }

		shared_ptr<BoundStatement> GetElse()
		{ return mElse; }
	};
}