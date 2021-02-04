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
			os.put('\n'); GetOperand()->Print(os, newIndent, true, "Operand = ");
		}

		shared_ptr<BoundUnaryOperator> GetOperator()
		{ return mOperator; }

		shared_ptr<BoundExpression> GetOperand()
		{ return mOperand; }
	};
}