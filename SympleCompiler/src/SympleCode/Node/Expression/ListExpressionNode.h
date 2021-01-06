#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"

#include <vector>

namespace Symple
{
	class ListExpressionNode : public ExpressionNode
	{
	private:
		const Token* mOpen;
		const std::vector<const ExpressionNode*> mExpressions;
		const Token* mClose;
	public:
		ListExpressionNode(const Token* open, const std::vector<const ExpressionNode*> expressions, const Token* close)
			: ExpressionNode(expressions.empty() ? VoidType : expressions[0]->GetType()), mOpen(open), mExpressions(expressions), mClose(close)
		{
			for (const ExpressionNode* expression : mExpressions)
				Diagnostics::ReportError(mType != expression->GetType(), mOpen, "Unmatched Types : \n % s, \n % s",
					mType->ToString("", false).c_str(), expression->GetType()->ToString().c_str());
		}

		Kind GetKind() const override
		{
			return Kind::ListExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "List Expression [" << mExpressions.size() << "]";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const ExpressionNode* expression : mExpressions)
				ss << '\n' << expression->ToString(indent + newIndent, expression == mExpressions.back());

			return ss.str();
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const Token* GetClose() const
		{
			return mClose;
		}

		const std::vector<const ExpressionNode*> GetExpression() const
		{
			return mExpressions;
		}
	};
}