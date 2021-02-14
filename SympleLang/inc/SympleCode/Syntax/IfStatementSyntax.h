#pragma once

#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/ParenthesizedExpressionSyntax.h"

namespace Symple::Syntax
{
	class IfStatementSyntax : public StatementSyntax
	{
	private:
		shared_ptr<ParenthesizedExpressionSyntax> mCondition;
		shared_ptr<StatementSyntax> mThen;
		shared_ptr<Token> mElseKeyword;
		shared_ptr<StatementSyntax> mElse;
	public:
		IfStatementSyntax(shared_ptr<Token> ifKey, shared_ptr<ParenthesizedExpressionSyntax> cond, shared_ptr<StatementSyntax> then, shared_ptr<Token> elseKey, shared_ptr<StatementSyntax> elze)
			: StatementSyntax(ifKey), mCondition(cond), mThen(then), mElseKeyword(elseKey), mElse(elze) {}

		virtual Kind GetKind() override
		{ return IfStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			
			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetCondition()->Print(os, newIndent, false, "Condition = ");
			os.put('\n'); GetThen()->Print(os, newIndent, !GetElse(), "Then = ");
			if (GetElse())
			{ os.put('\n'); GetElse()->Print(os, newIndent, true, "Else = "); }
		}

		shared_ptr<Token> GetIfKeyword()
		{ return GetToken(); }

		shared_ptr<ParenthesizedExpressionSyntax> GetCondition()
		{ return mCondition; }

		shared_ptr<StatementSyntax> GetThen()
		{ return mThen; }

		shared_ptr<Token> GetElseKeyword()
		{ return mElseKeyword; }

		shared_ptr<StatementSyntax> GetElse()
		{ return mElse; }
	};
}