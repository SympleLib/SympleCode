#pragma once

#include <map>

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Statement/StatementNode.h"
#include "SympleCode/Node/Statement/VariableDeclarationNode.h"

namespace Symple
{
	class BlockStatementNode : public StatementNode
	{
	private:
		const Token* mOpen;
		const std::vector<const StatementNode*> mStatements;
		const Token* mClose;

		std::map<std::string_view, const VariableDeclarationNode*> mVariables;
	public:
		BlockStatementNode(const Token* open, const std::vector<const StatementNode*>& statements, const Token* close)
			: mOpen(open), mStatements(statements), mClose(close)
		{
			for (const StatementNode* statement : mStatements)
				if (statement->Is<VariableDeclarationNode>())
					mVariables.insert({ statement->Cast<VariableDeclarationNode>()->GetName()->GetLex(), statement->Cast<VariableDeclarationNode>() });
		}

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

		const std::map<std::string_view, const VariableDeclarationNode*>& GetVariables() const
		{
			return mVariables;
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