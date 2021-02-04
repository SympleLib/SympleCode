#pragma once

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/MemberSyntax.h"

namespace Symple::Syntax
{
	class TranslationUnitSyntax : public Node
	{
	private:
		std::vector<shared_ptr<MemberSyntax>> mMembers;
	public:
		TranslationUnitSyntax(std::vector<shared_ptr<MemberSyntax>>& members, shared_ptr<Token> eof)
			: Node(eof), mMembers(members) {}

		virtual Kind GetKind() override
		{ return TranslationUnit; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto member : GetMembers())
			{ os.put('\n'); member->Print(os, newIndent, member == GetMembers().back()); }
		}

		std::vector<shared_ptr<MemberSyntax>> GetMembers()
		{ return mMembers; }
	};
}