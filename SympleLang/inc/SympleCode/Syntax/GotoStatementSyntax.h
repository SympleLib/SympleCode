#pragma once

#include "SympleCode/Syntax/StatementSyntax.h"

namespace Symple::Syntax
{
	class GotoStatementSyntax : public StatementSyntax
	{
	private:
		shared_ptr<Token> mKeyword;
	public:
		GotoStatementSyntax(shared_ptr<Token> key, shared_ptr<Token> label)
			: StatementSyntax(label), mKeyword(key) {}

		virtual Kind GetKind() override
		{ return GotoStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetLabel()->Print(os, newIndent, true, "Label = ");
		}

		shared_ptr<Token> GetKeyword()
		{ return mKeyword; }

		shared_ptr<Token> GetLabel()
		{ return GetToken(); }
	};
}