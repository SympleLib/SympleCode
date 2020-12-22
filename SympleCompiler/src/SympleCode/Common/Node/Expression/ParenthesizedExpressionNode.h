#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class ParenthesizedExpressionNode : public ExpressionNode
	{
	private:
		const Token* mOpen;
		const ExpressionNode* mExpression;
		const Token* mClose;
	public:
		ParenthesizedExpressionNode(const Token* open, const ExpressionNode* expression, const Token* close)
			: mOpen(open), mExpression(expression), mClose(close) {}

		Kind GetKind() const override
		{
			return Kind::ParenthesizedExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Parenthesized Expression";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mExpression->ToString(indent + newIndent);
			return ss.str();
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const ExpressionNode* GetExpression() const
		{
			return mExpression;
		}

		const Token* GetClose() const
		{
			return mClose;
		}
	};
}