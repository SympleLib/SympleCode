#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Statement/BlockStatementNode.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class IfStatementNode : public StatementNode
	{
	private:
		const Token* mOpen;
		const ExpressionNode* mCondition;
		const BlockStatementNode* mThen;
		const BlockStatementNode* mElse;
	public:
		IfStatementNode(const Token* open, const ExpressionNode* condition, const BlockStatementNode* then, const BlockStatementNode* elze)
			: mOpen(open), mCondition(condition), mThen(then), mElse(elze) {}

		Kind GetKind() const override
		{
			return Kind::IfStatement;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "If Statement:";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mCondition->ToString(indent + newIndent, false);
			ss << "\nThen\n" << mThen->ToString(indent + newIndent, !mElse);
			if (mElse)
				ss << "\nElse\n" << mElse->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const ExpressionNode* GetCondition() const
		{
			return mCondition;
		}

		const BlockStatementNode* GetThen() const
		{
			return mThen;
		}

		const BlockStatementNode* GetElse() const
		{
			return mElse;
		}
	};
}