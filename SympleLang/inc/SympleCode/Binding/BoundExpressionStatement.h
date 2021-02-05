#pragma once

#include "SympleCode/Syntax/ExpressionStatementSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"
#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundExpressionStatement : public BoundStatement
	{
	private:
		shared_ptr<BoundExpression> mExpression;
	public:
		BoundExpressionStatement(shared_ptr<Syntax::ExpressionStatementSyntax> syntax, shared_ptr<BoundExpression> expr)
			: BoundStatement(syntax), mExpression(expr) {}

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

		shared_ptr<BoundExpression> GetExpression()
		{ return mExpression; }
	};
}