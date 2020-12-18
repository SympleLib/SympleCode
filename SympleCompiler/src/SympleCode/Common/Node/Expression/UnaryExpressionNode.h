#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class UnaryExpressionNode : public ExpressionNode
	{
	private:
		const Token* mOperator;
		const ExpressionNode* mValue;
	public:
		UnaryExpressionNode(const Token* oqerator, const ExpressionNode* value)
			: mOperator(oqerator), mValue(value) {}

		Kind GetKind() const override
		{
			return Kind::UnaryExpression;
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

		const Token* GetOperator() const { return mOperator; }

		const ExpressionNode* GetValue() const { return mValue; }
	};
}