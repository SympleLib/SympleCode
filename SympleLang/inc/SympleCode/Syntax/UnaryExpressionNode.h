#pragma once

#include "SympleCode/Syntax/ExpressionNode.h"

namespace Symple::Syntax
{
	class UnaryExpressionNode : public ExpressionNode
	{
	private:
		shared_ptr<ExpressionNode> mOperand;
	public:
		UnaryExpressionNode(shared_ptr<Token> op, shared_ptr<ExpressionNode> operand)
			: ExpressionNode(op), mOperand(operand) {}

		virtual Kind GetKind()
		{
			return UnaryExpression;
		}

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Unary Expression Node [";
			GetOperator()->PrintShort(os);
			os.put(']');

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetOperand()->Print(os, newIndent, true, "Operand = ");
		}

		shared_ptr<Token> GetOperator()
		{ return GetToken(); }

		shared_ptr<ExpressionNode> GetOperand()
		{ return mOperand; }
	};
}