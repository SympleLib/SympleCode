#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class TernaryExpressionNode : public ExpressionNode
	{
	private:
		const ExpressionNode* mCondition;
		const Token* mQuestionMark;
		const ExpressionNode* mThen;
		const Token* mColon;
		const ExpressionNode* mElse;
	public:
		TernaryExpressionNode(const ExpressionNode* condition, const Token* questionMark, const ExpressionNode* then, const Token* colon, const ExpressionNode* elze)
			: ExpressionNode(then->GetType()), mCondition(condition), mQuestionMark(questionMark), mThen(then), mColon(colon), mElse(elze)
		{
			Diagnostics::ReportError(!mElse->GetType()->CanImplicitlyCastTo(mThen->GetType()), mColon, "Unmatched Types:\n%s,\n%s",
				mThen->ToString("", false).c_str(), mElse->GetType()->ToString().c_str());
		}

		Kind GetKind() const override
		{
			return Kind::TernaryExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Ternary Expression";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mCondition->ToString(indent + newIndent, false);
			ss << "\nThen\n" << mThen->ToString(indent + newIndent, !mElse);
			if (mElse)
				ss << "\nElse\n" << mElse->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetQuestionMark() const
		{
			return mQuestionMark;
		}

		const Token* GetColon() const
		{
			return mColon;
		}

		const ExpressionNode* GetCondition() const
		{
			return mCondition;
		}

		const ExpressionNode* GetThen() const
		{
			return mThen;
		}

		const ExpressionNode* GetElse() const
		{
			return mElse;
		}
	};
}