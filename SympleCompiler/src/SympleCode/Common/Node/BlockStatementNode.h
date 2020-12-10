#pragma once

#include <vector>

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/StatementNode.h"

namespace Symple
{
	class BlockStatementNode : public StatementNode
	{
	private:
		const Token* mOpen;
		const std::vector<const StatementNode*> mStatements;
		const Token* mClose;
	public:
		BlockStatementNode(const Token* open, const std::vector<const StatementNode*>& statements, const Token* close)
			: mOpen(open), mStatements(statements), mClose(close) {}

		Kind GetKind() const override
		{
			return Kind::BlockStatement;
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const std::vector<const StatementNode*>& GetStatements() const
		{
			return mStatements;
		}

		const Token* GetClose() const
		{
			return mClose;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Block Statement";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const StatementNode* statement : mStatements)
				ss << '\n' << statement->ToString(indent + newIndent, statement == mStatements.back());

			return ss.str();
		}
	};
}