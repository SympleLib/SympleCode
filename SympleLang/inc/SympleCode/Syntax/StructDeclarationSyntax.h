#pragma once

#include <vector>

#include "SympleCode/Syntax/MemberSyntax.h"
#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/VariableDeclarationSyntax.h"
#include "SympleCode/Syntax/TypeSyntax.h"

namespace Symple::Syntax
{
	class StructDeclarationSyntax: public MemberSyntax
	{
	private:
		shared_ptr<Token> mKeyword;
		shared_ptr<Token> mOpenBrace;
		VariableDeclarationList mMembers;
		shared_ptr<Token> mCloseBrace;
	public:
		StructDeclarationSyntax(shared_ptr<Token> keyword, shared_ptr<Token> name, shared_ptr<Token> openBrace,
			VariableDeclarationList members, shared_ptr<Token> closeBrace)
			: MemberSyntax(name), mKeyword(keyword), mOpenBrace(openBrace), mMembers(members), mCloseBrace(closeBrace)
		{}

		virtual Kind GetKind() override
		{ return StructDeclaration; }

		virtual void Print(std::ostream &os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '" << GetName()->GetText() << "'";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto member : GetMembers())
			{ os.put('\n'); member->Print(os, newIndent, member == GetMembers().back(), "[Member] "); }
		}

		shared_ptr<Token> GetKeyword()
		{ return mKeyword; }

		shared_ptr<Token> GetName()
		{ return GetToken(); }

		shared_ptr<Token> GetOpenBrace()
		{ return mOpenBrace; }

		VariableDeclarationList GetMembers()
		{ return mMembers; }

		shared_ptr<Token> GetCloseBrace()
		{ return mCloseBrace; }
	};
}