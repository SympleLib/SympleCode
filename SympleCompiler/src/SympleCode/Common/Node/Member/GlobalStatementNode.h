#pragma once

#include "SympleCode/Common/Node/Statement/StatementNode.h"
#include "SympleCode/Common/Node/Member/MemberNode.h"

namespace Symple
{
	class GlobalStatementNode : public MemberNode
	{
	private:
		const StatementNode* mStatement;
	public:
		GlobalStatementNode(const StatementNode* statement)
			: mStatement(statement) {}

		Kind GetKind() const override
		{
			return Kind::GlobalStatement;
		}

		const StatementNode* GetStatement() const
		{
			return mStatement;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Global Statement";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mStatement->ToString(indent + newIndent);
			return ss.str();
		}
	};
}