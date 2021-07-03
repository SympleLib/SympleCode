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
		};
	}

	bool SymbolBuddy::IsTypeBase(GlobalRef<const Token> tok)
	{
		for (TypeBaseRef base : typeBases)
			if (base->Name == tok->Text)
				return true;
		return false;
	}

	TypeBaseRef SymbolBuddy::GetTypeBase(GlobalRef<const Token> tok)
	{
		for (TypeBaseRef base : typeBases)
			if (base->Name == tok->Text)
				return base;
		throw nullptr;
	}
}