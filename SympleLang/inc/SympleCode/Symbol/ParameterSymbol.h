#pragma once

#include "SympleCode/Symbol/Symbol.h"
#include "SympleCode/Symbol/TypeSymbol.h"

#include "SympleCode/Binding/BoundConstant.h"

namespace Symple::Symbol
{
	class ParameterSymbol : public Symbol
	{
	private:
		shared_ptr<TypeSymbol> mType;
		std::string mName;
		shared_ptr<Binding::BoundConstant> mInitializer;
	public:
		ParameterSymbol(shared_ptr<TypeSymbol> ty, std::string_view name, shared_ptr<Binding::BoundConstant> init)
			: mType(ty), mName(name), mInitializer(init)
		{}

		virtual Kind GetKind() override
		{ return Parameter; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " '"; GetType()->PrintShort(os); os << ' ' << GetName();
			if (GetInitializer())
			{
				os << " = ";
				GetInitializer()->PrintShort(os);
			}
			os.put('\'');
		}

		void PrintShort(std::ostream& os = std::cout)
		{
			GetType()->PrintShort(os); os << ' ' << GetName();
			if (GetInitializer())
			{
				os << " = ";
				GetInitializer()->PrintShort(os);
			}
		}

		shared_ptr<TypeSymbol> GetType()
		{ return mType; }

		std::string_view GetName()
		{ return mName; }

		shared_ptr<Binding::BoundConstant> GetInitializer()
		{ return mInitializer; }
	};

	typedef std::vector<shared_ptr<ParameterSymbol>> ParameterList;
}