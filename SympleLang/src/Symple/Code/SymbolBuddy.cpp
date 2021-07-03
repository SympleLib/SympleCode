#include "Symple/Code/SymbolBuddy.h"

namespace Symple::Code
{
	bool SymbolBuddy::IsTypeBase(GlobalRef<const Token> tok)
	{
		for (TypeBaseRef base : typeBases)
			if (base.Name == tok->Text)
				return true;
		return false;
	}

	TypeBaseRef SymbolBuddy::GetTypeBase(GlobalRef<const Token> tok)
	{
		for (TypeBaseRef base : typeBases)
			if (base.Name == tok->Text)
				return base;
		throw nullptr;
	}
}