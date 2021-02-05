#pragma once

#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class ExpressionStatementSyntax : public StatementSyntax
	{
	private:
		shared_ptr<ExpressionSyntax> mExpression;
	public:
		ExpressionStatementSyntax(shared_ptr<ExpressionSyntax> expr)
			: StatementSyntax(expr->GetToken()), mExpression(expr) {}

		virtual Kind GetKind() override
		{ return ExpressionStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetExpression()->Print(os, newIndent, true, "Expression = ");
		}

		shared_ptr<ExpressionSyntax> GetExpression()
		{ return mExpression; }
	};
}