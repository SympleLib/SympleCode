#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/ExpressionNode.h"

namespace Symple
{
	class BinaryExpressonNode : public ExpressionNode
	{
	private:
		const Token* mOperator;
		const ExpressionNode* mLeft;
		const ExpressionNode* mRight;
	public:
		BinaryExpressonNode(const Token* oqerator, const ExpressionNode* left, const ExpressionNode* right)
			: mOperator(oqerator), mLeft(left), mRight(right) {}

		Kind GetKind() const override
		{
			return Kind::BinaryExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Binary Expression [" << mOperator->GetLex() << "]";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mLeft->ToString(indent + newIndent, false);
			ss << '\n' << mRight->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetOperator() const { return mOperator; }
		
        const ExpressionNode* getLeft() const { return mLeft; }

        const ExpressionNode* getRight() const { return mRight; }
};
}