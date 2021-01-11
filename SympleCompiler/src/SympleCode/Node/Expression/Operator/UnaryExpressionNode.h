#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Expression/Operator/OperatorExpressionNode.h"

namespace Symple
{
	class UnaryExpressionNode : public OperatorExpressionNode
	{
	private:
		const ExpressionNode* mValue;

		int mEvaluate;
	public:
		UnaryExpressionNode(const Token* oqerator, const ExpressionNode* value)
			: OperatorExpressionNode(value->GetType(), oqerator), mValue(value),
				mEvaluate()
		{
			switch (mOperator->GetKind())
			{
			case Token::Kind::Exclamation:
				mEvaluate = !mValue->Evaluate();
				break;
			case Token::Kind::Minus:
				mEvaluate = -mValue->Evaluate();
				break;
			default:
				Diagnostics::ReportError(mOperator, "Invalid Operation!");
			}
		}

		Kind GetKind() const override
		{
			return Kind::UnaryExpression;
		}

		bool CanEvaluate() const override
		{
			return mValue->CanEvaluate();
		}

		int Evaluate() const override
		{
			return mEvaluate;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "UnaryExpression Expression [" << mOperator->GetLex() << "]";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mValue->ToString(indent + newIndent);

			return ss.str();
		}

		const ExpressionNode* GetValue() const { return mValue; }
	};
}