#pragma once

#include "SympleCode/Symbol/Symbol.h"
#include "SympleCode/Symbol/VariableSymbol.h"

namespace Symple::Binding
{
	class BoundScope
	{
	private:
		shared_ptr<BoundScope> mBase;
		
		Symbol::VariableList mVariables;
	public:
		BoundScope(shared_ptr<BoundScope> base = nullptr)
			: mBase(base) {}

		void DeclareVariable(shared_ptr<Symbol::VariableSymbol> var)
		{ mVariables.push_back(var); }

		shared_ptr<BoundScope> GetBase()
		{ return mBase; }

		Symbol::VariableList GetDeclaredVariables()
		{ return mVariables; }

		shared_ptr<Symbol::VariableSymbol> GetVariableSymbol(std::string_view name)
		{
			for (auto var : GetDeclaredVariables())
				if (var->GetName() == name)
					return var;
			if (GetBase())
				return GetBase()->GetVariableSymbol(name);
			else
				return nullptr;
		}
	};
}