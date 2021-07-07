#include "Symple/Code/SymbolBuddy.h"

namespace Symple::Code
{
	SymbolBuddy::SymbolBuddy()
	{
		typeBases = {
			(TypeBaseRef)NativeType::Void,
			(TypeBaseRef)NativeType::Byte, (TypeBaseRef)NativeType::Short, (TypeBaseRef)NativeType::Int, (TypeBaseRef)NativeType::Long,
			(TypeBaseRef)NativeType::Float, (TypeBaseRef)NativeType::Double,
			(TypeBaseRef)NativeType::Char, (TypeBaseRef)NativeType::WChar,
			(TypeBaseRef)NativeType::Bool,
		};
	}

	void SymbolBuddy::DefineTypeBase(TypeBaseRef ty)
	{ typeBases.push_back(ty); }

	bool SymbolBuddy::IsTypeBase(GlobalRef<const Token> tok)
	{
		for (TypeBaseRef base : typeBases)
			if (base->TypeName == tok->Text)
				return true;
		return false;
	}

	TypeBaseRef SymbolBuddy::GetTypeBase(GlobalRef<const Token> tok)
	{
		for (TypeBaseRef base : typeBases)
			if (base->TypeName == tok->Text)
				return base;
		throw nullptr;
	}
}