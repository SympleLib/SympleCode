#pragma once

#include "SympleCode/Symbol/TypeSymbol.h"
#include "SympleCode/Symbol/MemberSymbol.h"

namespace Symple::Symbol
{
	class StructTypeSymbol: public TypeSymbol
	{
	private:
		MemberList mMembers;
	public:
		StructTypeSymbol(std::string_view name, unsigned sz, MemberList members)
			: TypeSymbol(Struct, name, sz), mMembers(members) {}

		virtual Kind GetKind() override
		{ return StructType; }

		virtual void Print(std::ostream &os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " '" << GetName() << '\'';

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto member : GetMembers())
			{ os << '\n'; member->Print(os, newIndent, member == GetMembers().back(), "[Member] "); }
		}

		void PrintShort(std::ostream &os = std::cout)
		{
			os << GetName() << " { ";
			for (auto member : GetMembers())
			{
				member->PrintShort(os);
				if (member != GetMembers().back())
					os << ", ";
			}
			os << " }";
		}

		MemberList GetMembers()
		{ return mMembers; }
	};
}