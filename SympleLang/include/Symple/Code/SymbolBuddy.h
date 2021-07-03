#pragma once

#include "Symple/Code/Type/Type.h"
#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	struct SYC_API SymbolBuddy
	{
		std::vector<TypeBase> typeBases = {
			NativeType::Void,
			NativeType::Byte, NativeType::Short, NativeType::Int, NativeType::Long,
			NativeType::Float, NativeType::Double,
			NativeType::Char, NativeType::WChar,
		};

		bool IsTypeBase(GlobalRef<const Token>);
		TypeBaseRef GetTypeBase(GlobalRef<const Token>);
	};
}