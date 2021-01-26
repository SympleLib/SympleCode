#pragma once

#include "SympleCode/Syntax/ExpressionNode.h"

namespace Symple::Syntax
{
	class BinaryExpressionNode : public ExpressionNode
	{
	private:
		shared_ptr<ExpressionNode> mLeft, mRight;
	public:
		BinaryExpressionNode(shared_ptr<ExpressionNode> left, shared_ptr<Token> op, shared_ptr<ExpressionNode> right)
			: ExpressionNode(op), mLeft(left), mRight(right) {}

		virtual Kind GetKind()
		{
			return BinaryExpression;
		}

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Binary Expression Node [";
			GetOperator()->PrintShort(os);
			os.put(']');

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetLeft()->Print(os, newIndent, false, "Left = ");
			os.put('\n'); GetRight()->Print(os, newIndent, true, "Right = ");
		}

		virtual bool CanEvaluate()
		{ return mLeft->CanEvaluate() && mRight->CanEvaluate(); }

		virtual int Evaluate()
		{
			switch (GetOperator()->GetKind())
			{
			case Token::Plus:
				return mLeft->Evaluate() + mRight->Evaluate();
			case Token::Dash:
				return mLeft->Evaluate() - mRight->Evaluate();

			case Token::Asterisk:
				return mLeft->Evaluate() * mRight->Evaluate();
			case Token::Slash:
				return mLeft->Evaluate() / mRight->Evaluate();
			}

			return 0;
		}

		shared_ptr<Token> GetOperator()
		{ return GetToken(); }

		shared_ptr<ExpressionNode> GetLeft()
		{ return mLeft; }

		shared_ptr<ExpressionNode> GetRight()
		{ return mRight; }
	};
}