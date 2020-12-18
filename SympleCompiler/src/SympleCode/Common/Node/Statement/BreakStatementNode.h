#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Statement/StatementNode.h"

namespace Symple
{
	class BreakStatementNode : public StatementNode
	{
	private:
		const Token* mStatement;
	public:
		BreakStatementNode(const Token* statement)
			: mStatement(statement) {}

		Kind GetKind() const override
		{
			return Kind::BreakStatement;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Break Statement";

			return ss.str();
		}

		const Token* GetStatement() const
		{
			return mStatement;
		}
	};
}