#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Statement/BlockStatementNode.h"
#include "SympleCode/Common/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class WhileStatementNode : public StatementNode
	{
	private:
		const Token* mOpen;
		const ExpressionNode* mCondition;
		const BlockStatementNode* mBody;
	public:
		WhileStatementNode(const Token* open, const ExpressionNode* condition, const BlockStatementNode* body)
			: mOpen(open), mCondition(condition), mBody(body) {}

		Kind GetKind() const override
		{
			return Kind::WhileStatement;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "While Statement:";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mCondition->ToString(indent + newIndent, false);
			ss << '\n' << mBody->ToString(indent + newIndent);

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

		const BlockStatementNode* GetBody() const
		{
			return mBody;
		}
	};
}