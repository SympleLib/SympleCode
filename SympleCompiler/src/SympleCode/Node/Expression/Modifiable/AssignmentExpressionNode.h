#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Expression/Modifiable/ModifiableExpressionNode.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class AssignmentExpressionNode : public ModifiableExpressionNode
	{
	private:
		const Token* mOperator;
		const ModifiableExpressionNode* mLeft;
		const ExpressionNode* mRight;
	public:
		AssignmentExpressionNode(const Token* oqerator, const ModifiableExpressionNode* left, const ExpressionNode* right)
			: ModifiableExpressionNode(left->GetType()), mOperator(oqerator), mLeft(left), mRight(right)
		{
			Diagnostics::ReportError(!left->IsMutable(), mOperator, "Left Hand Side is not Mutable");
			Diagnostics::ReportError(!mRight->GetType()->CanImplicitlyCastTo(mLeft->GetType()), mOperator, "Unmatched Types:\n%s,\n%s",
				mLeft->GetType()->ToString("", false).c_str(), mRight->GetType()->ToString().c_str());
		}
		Kind GetKind() const override
		{
			return Kind::AssignmentExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Assignment Expression [" << mOperator->GetLex() << "]";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mLeft->ToString(indent + newIndent, false);
			ss << '\n' << mRight->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetOperator() const { return mOperator; }

		const ModifiableExpressionNode* GetLeft() const { return mLeft; }

		const ExpressionNode* GetRight() const { return mRight; }

		bool IsMutable() const { return true; }
	};
}