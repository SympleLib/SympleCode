#pragma once

#include "SympleCode/Syntax/UnaryExpressionSyntax.h"

#include "SympleCode/Binding/BoundUnaryOperator.h"
#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundUnaryExpression : public BoundExpression
	{
	private:
		shared_ptr<BoundUnaryOperator> mOperator;
		shared_ptr<BoundExpression> mOperand;
	public:
		BoundUnaryExpression(shared_ptr<Syntax::UnaryExpressionSyntax> syntax, shared_ptr<BoundUnaryOperator> oqerator, shared_ptr<BoundExpression> operand)
			: BoundExpression(syntax), mOperator(oqerator), mOperand(operand)
		{}

		virtual Kind GetKind() override
		{ return UnaryExpression; }

		virtual shared_ptr<Type> GetType() override
		{ return mOperator->GetType(); }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			os << "Unary Expression Syntax [";
			GetOperator()->PrintShort(os);
			os.put(']');

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetOperand()->Print(os, newIndent, false, "Operand = ");
		}

		shared_ptr<BoundUnaryOperator> GetOperator()
		{
			return mOperator;
		}

		shared_ptr<BoundExpression> GetOperand()
		{ return mOperand; }
	};
}