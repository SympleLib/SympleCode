#pragma once

#include <vector>

#include "SympleCode/Syntax/StatementSyntax.h"

namespace Symple::Syntax
{
	class BlockStatementSyntax : public StatementSyntax
	{
	private:
		std::vector<shared_ptr<StatementSyntax>> mStatements;
		shared_ptr<Token> mClose;
	public:
		BlockStatementSyntax(shared_ptr<Token> open, std::vector<shared_ptr<StatementSyntax>> statements, shared_ptr<Token> close)
			: StatementSyntax(open), mStatements(statements), mClose(close) {}

		virtual Kind GetKind() override
		{ return BlockStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto statement : GetStatements())
			{ os.put('\n'); statement->Print(os, newIndent, statement == GetStatements().back()); }
		}

		shared_ptr<Token> GetOpen()
		{ return GetToken(); }

		std::vector<shared_ptr<StatementSyntax>> GetStatements()
		{ return mStatements; }

		shared_ptr<Token> GetClose()
		{ return mClose; }
	};
}