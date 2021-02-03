#pragma once

#include "SympleCode/Syntax/BlockStatementSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"

namespace Symple::Binding
{
	class BoundBlockStatement : public BoundStatement
	{
	private:
		std::vector<shared_ptr<BoundStatement>> mStatements;
	public:
		BoundBlockStatement(shared_ptr<Syntax::BlockStatementSyntax> syntax, std::vector<shared_ptr<BoundStatement>> statements)
			: BoundStatement(syntax), mStatements(statements) {}

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

		std::vector<shared_ptr<BoundStatement>> GetStatements()
		{ return mStatements; }
	};
}