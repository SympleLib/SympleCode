#pragma once

#include "SympleCode/Syntax/MemberSyntax.h"

namespace Symple::Syntax
{
	class ImportStatementSyntax : public MemberSyntax
	{
	private:
		shared_ptr<Token> mKeyword;
	public:
		ImportStatementSyntax(shared_ptr<Token> key, shared_ptr<Token> import)
			: MemberSyntax(import), mKeyword(key) {}

		virtual Kind GetKind() override
		{ return ImportStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetToken()->Print(os, newIndent, true, "Import = ");
		}

		shared_ptr<Token> GetKeyword()
		{ return mKeyword; }

		shared_ptr<Token> GetImport()
		{ return GetToken(); }
	};
}