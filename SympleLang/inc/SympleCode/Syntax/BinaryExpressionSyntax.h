#pragma once

#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class BinaryExpressionSyntax : public ExpressionSyntax
	{
	private:
		shared_ptr<ExpressionSyntax> mLeft, mRight;
	public:
		BinaryExpressionSyntax(shared_ptr<Token> op, shared_ptr<ExpressionSyntax> left, shared_ptr<ExpressionSyntax> right)
			: ExpressionSyntax(op), mLeft(left), mRight(right) {}

		virtual Kind GetKind() override
		{ return BinaryExpression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os); 
			os << " [";
			GetOperator()->PrintShort(os);
			os.put(']');

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetLeft()->Print(os, newIndent, false, "Left = ");
			os.put('\n'); GetRight()->Print(os, newIndent, true, "Right = ");
		}

		shared_ptr<Token> GetOperator()
		{ return GetToken(); }

		shared_ptr<ExpressionSyntax> GetLeft()
		{ return mLeft; }

		shared_ptr<ExpressionSyntax> GetRight()
		{ return mRight; }
	};
}