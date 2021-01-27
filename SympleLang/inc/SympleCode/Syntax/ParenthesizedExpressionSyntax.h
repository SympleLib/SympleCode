#pragma once

#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class ParenthesizedExpressionSyntax : public ExpressionSyntax
	{
	private:
		shared_ptr<ExpressionSyntax> mExpression;
		shared_ptr<Token> mClose;
	public:
		ParenthesizedExpressionSyntax(shared_ptr<Token> open, shared_ptr<ExpressionSyntax> expression, shared_ptr<Token> close)
			: ExpressionSyntax(open), mExpression(expression), mClose(close) {}

		virtual Kind GetKind()
		{ return ParenthesizedExpression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Parenthesized Expression Syntax";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetExpression()->Print(os, newIndent, true, "Expression = ");
		}

		shared_ptr<Token> GetOpen()
		{ return GetToken(); }

		shared_ptr<ExpressionSyntax> GetExpression()
		{ return mExpression; }

		shared_ptr<Token> GetClose()
		{ return mClose; }
	};
}