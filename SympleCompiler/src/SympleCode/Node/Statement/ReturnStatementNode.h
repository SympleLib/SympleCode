#pragma once

#include "SympleCode/Node/Statement/StatementNode.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class ReturnStatementNode : public StatementNode
	{
	private:
		const ExpressionNode* mExpression;
	public:
		ReturnStatementNode(const ExpressionNode* expression)
			: mExpression(expression) {}

		Kind GetKind() const
		{
			return Kind::ReturnStatement;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Return Statement";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mExpression->ToString(indent + newIndent);
			return ss.str();
		}

		const ExpressionNode* GetExpression() const
		{
			return mExpression;
		}
	};
}