#pragma once

#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class UnaryExpressionSyntax : public ExpressionSyntax
	{
	private:
		shared_ptr<ExpressionSyntax> mOperand;
	public:
		UnaryExpressionSyntax(shared_ptr<Token> op, shared_ptr<ExpressionSyntax> operand)
			: ExpressionSyntax(op), mOperand(operand) {}

		virtual Kind GetKind()
		{ return UnaryExpression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Unary Expression Syntax [";
			GetOperator()->PrintShort(os);
			os.put(']');

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetOperand()->Print(os, newIndent, true, "Operand = ");
		}

		shared_ptr<Token> GetOperator()
		{ return GetToken(); }

		shared_ptr<ExpressionSyntax> GetOperand()
		{ return mOperand; }
	};
}