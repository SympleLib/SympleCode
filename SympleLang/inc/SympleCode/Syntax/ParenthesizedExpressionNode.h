#pragma once

#include "SympleCode/Syntax/ExpressionNode.h"

namespace Symple::Syntax
{
	class ParenthesizedExpressionNode : public ExpressionNode
	{
	private:
		shared_ptr<ExpressionNode> mExpression;
		shared_ptr<Token> mClose;
	public:
		ParenthesizedExpressionNode(shared_ptr<Token> open, shared_ptr<ExpressionNode> expression, shared_ptr<Token> close)
			: ExpressionNode(open), mExpression(expression), mClose(close) {}

		virtual Kind GetKind()
		{ return ParenthesizedExpression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Parenthesized Expression Node";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetExpression()->Print(os, newIndent, true, "Expression = ");
		}

		shared_ptr<Token> GetOpen()
		{ return GetToken(); }

		shared_ptr<ExpressionNode> GetExpression()
		{ return mExpression; }

		shared_ptr<Token> GetClose()
		{ return mClose; }
	};
}