#pragma once

#include "SympleCode/Symbol/Symbol.h"
#include "SympleCode/Symbol/VariableSymbol.h"

namespace Symple::Emit
{
	class Scope
	{
	private:
		shared_ptr<Scope> mBase;
		unsigned mDepth;
		
		Symbol::VariableList mVariables;
	public:
		Scope(shared_ptr<Scope> base = nullptr)
			: mBase(base), mDepth(base ? base->mDepth : 0) {}

		void DeclareVariable(shared_ptr<Symbol::VariableSymbol> var)
		{ mVariables.push_back(var); }

		shared_ptr<Scope> GetBase()
		{ return mBase; }

		unsigned GetDepth()
		{ return mDepth; }

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