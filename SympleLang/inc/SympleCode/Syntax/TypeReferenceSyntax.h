#pragma once

#include "SympleCode/Syntax/TypeSyntax.h"

namespace Symple::Syntax
{
	class TypeReferenceSyntax : public TypeSyntax
	{
	private:
		shared_ptr<TypeSyntax> mBase;
	public:
		TypeReferenceSyntax(shared_ptr<Token> name, shared_ptr<TypeSyntax> base)
			: TypeSyntax(name), mBase(base) {}

		virtual Kind GetKind() override
		{ return TypeReference; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			if (GetBase())
			{
				std::string newIndent(indent);
				newIndent += GetAddIndent(last);

				GetBase()->Print(os, newIndent, true, "Base = ");
			}
		}

		virtual void PrintShort(std::ostream& os = std::cout) override
		{
			if (GetBase())
			{
				GetBase()->PrintShort();
				os.put(' ');
			}

			TypeSyntax::PrintShort(os);
		}

		shared_ptr<TypeSyntax> GetBase()
		{ return mBase; }
	};
}