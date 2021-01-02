#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class OperatorExpressionNode : public ExpressionNode
	{
	protected:
		const Token* mOperator;
	public:
		OperatorExpressionNode(const TypeNode* type, const Token* oqerator)
			: ExpressionNode(type), mOperator(oqerator) {}

		Kind GetKind() const
		{
			return Kind::OperatorExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Operator Expression [" << mOperator->GetLex() << "]";

			return ss.str();
		}

		const Token* GetOperator() const { return mOperator; }
	};
}