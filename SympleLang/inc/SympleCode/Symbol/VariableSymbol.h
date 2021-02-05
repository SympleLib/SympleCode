#pragma once

#include "SympleCode/Symbol/Symbol.h"
#include "SympleCode/Symbol/TypeSymbol.h"

namespace Symple::Symbol
{
	class VariableSymbol : public Symbol
	{
	private:
		shared_ptr<TypeSymbol> mType;
		std::string mName;
	public:
		VariableSymbol(shared_ptr<TypeSymbol> ty, std::string_view name)
			: mType(ty), mName(name)
		{}

		virtual Kind GetKind() override
		{ return Variable; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " '"; GetType()->PrintShort(os); os << ' ' << GetName() << '\'';
		}

		void PrintShort(std::ostream& os = std::cout)
		{ GetType()->PrintShort(os); os << ' ' << GetName(); }

		shared_ptr<TypeSymbol> GetType()
		{ return mType; }

		std::string_view GetName()
		{ return mName; }
	};

	typedef std::vector<shared_ptr<VariableSymbol>> VariableList;
}