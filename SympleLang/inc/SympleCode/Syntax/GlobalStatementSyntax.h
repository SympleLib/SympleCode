#pragma once

#include "SympleCode/Syntax/MemberSyntax.h"
#include "SympleCode/Syntax/StatementSyntax.h"

namespace Symple::Syntax
{
	class GlobalStatementSyntax : public MemberSyntax
	{
	private:
		shared_ptr<StatementSyntax> mStatement;
	public:
		GlobalStatementSyntax(shared_ptr<StatementSyntax> stmt)
			: MemberSyntax(stmt->GetToken()), mStatement(stmt) {}

		virtual Kind GetKind() override
		{ return GlobalStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetStatement()->Print(os, newIndent, true, "Value = ");
		}

		shared_ptr<StatementSyntax> GetStatement()
		{ return mStatement; }
	};
}