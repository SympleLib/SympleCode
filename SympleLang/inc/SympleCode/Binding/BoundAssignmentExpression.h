#pragma once

#include "SympleCode/Syntax/BinaryExpressionSyntax.h"

#include "SympleCode/Binding/BoundExpression.h"
#include "SympleCode/Binding/BoundBinaryOperator.h"

namespace Symple::Binding
{
	class BoundAssignmentExpression : public BoundExpression
	{
	private:
		shared_ptr<BoundExpression> mLeft, mRight;
	public:
		BoundAssignmentExpression(shared_ptr<Syntax::BinaryExpressionSyntax> syntax, shared_ptr<BoundExpression> left, shared_ptr<BoundExpression> right)
			: BoundExpression(syntax), mLeft(left), mRight(right)
		{}

		virtual Kind GetKind() override
		{ return AssignmentExpression; }

		virtual shared_ptr<Symbol::TypeSymbol> GetType() override
		{ return mOperator->GetType(); }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetType()->Print(os, newIndent, false, "Type = ");
			os.put('\n'); GetOperator()->Print(os, newIndent, false, "Operator = ");
			os.put('\n'); GetLeft()->Print(os, newIndent, false, "Left = ");
			os.put('\n'); GetRight()->Print(os, newIndent, true, "Right = ");
		}

		shared_ptr<BoundBinaryOperator> GetOperator()
		{ return ; }

		shared_ptr<BoundExpression> GetLeft()
		{ return mLeft; }

		shared_ptr<BoundExpression> GetRight()
		{ return mRight; }
	};
}