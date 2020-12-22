#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Expression/VariableExpressionNode.h"
#include "SympleCode/Common/Node/Expression/ModifiableExpressionNode.h"

namespace Symple
{
	class AssignmentExpressionNode : public OperatorExpressionNode, public ModifiableExpressionNode
	{
	private:
		const ModifiableExpressionNode* mLeft;
		const ExpressionNode* mRight;
	public:
		AssignmentExpressionNode(const Token* oqerator, const ModifiableExpressionNode* left, const ExpressionNode* right)
			: OperatorExpressionNode(oqerator), mLeft(left), mRight(right) {}

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

		const ModifiableExpressionNode* GetLeft() const { return mLeft; }

		const ExpressionNode* GetRight() const { return mRight; }
	};
}