#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Statement/BlockStatementNode.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class ForLoopStatementNode : public StatementNode
	{
	private:
		const Token* mOpen;
		const StatementNode* mInitializer;
		const ExpressionNode* mCondition;
		const StatementNode* mStep;
		const BlockStatementNode* mBody;
	public:
		ForLoopStatementNode(const Token* open, const StatementNode* initializer, const ExpressionNode* condition, const StatementNode* step, const BlockStatementNode* body)
			: mOpen(open), mInitializer(initializer), mCondition(condition), mStep(step), mBody(body) {}

		Kind GetKind() const override
		{
			return Kind::ForLoopStatement;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "For Loop Statement:";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mInitializer->ToString(indent + newIndent, false);
			ss << '\n' << mCondition->ToString(indent + newIndent, false);
			ss << '\n' << mStep->ToString(indent + newIndent, false);
			ss << '\n' << mBody->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const StatementNode* GetInitializer() const
		{
			return mInitializer;
		}

		const StatementNode* GetStep() const
		{
			return mStep;
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