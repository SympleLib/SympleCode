#pragma once

#include "SympleCode/Symbol/Symbol.h"
#include "SympleCode/Symbol/TypeSymbol.h"
#include "SympleCode/Symbol/ParameterSymbol.h"

namespace Symple::Symbol
{
	class FunctionSymbol : public Symbol
	{
	private:
		shared_ptr<TypeSymbol> mType;
		std::string mName;
		ParameterList mParameters;
	public:
		FunctionSymbol(shared_ptr<TypeSymbol> ty, std::string_view name, ParameterList& params)
			: mType(ty), mName(name), mParameters(params)
		{}

		virtual Kind GetKind() override
		{ return Function; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " '" << GetName() << '\'';

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);
			
			os.put('\n'); GetType()->Print(os, newIndent, false, "Return Type = ");
			for (auto param : GetParameters())
			{ os.put('\n'); param->Print(os, newIndent, param == GetParameters().back()); }
		}

		shared_ptr<TypeSymbol> GetType()
		{ return mType; }

		std::string_view GetName()
		{ return mName; }

		ParameterList GetParameters()
		{ return mParameters; }
	};
}