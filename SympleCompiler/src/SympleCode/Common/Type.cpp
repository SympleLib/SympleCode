#include "SympleCode/Common/Type.h"

namespace Symple
{
	const std::vector<const Type*> Type::PrimitiveTypes = {
		Type::PrimitiveType::Void,
		Type::PrimitiveType::Byte,
		Type::PrimitiveType::Short,
		Type::PrimitiveType::Int,

		Type::PrimitiveType::Bool,
		Type::PrimitiveType::Char,
		Type::PrimitiveType::WChar,
	};

	const Type* const Type::PrimitiveType::Void = new Type("void", 0);
	const Type* const Type::PrimitiveType::Byte = new Type("byte", 1);
	const Type* const Type::PrimitiveType::Short = new Type("short", 2);
	const Type* const Type::PrimitiveType::Int = new Type("int", 4);

	const Type* const Type::PrimitiveType::Bool = new Type("bool", 1);
	const Type* const Type::PrimitiveType::Char = new Type("char", 1);
	const Type* const Type::PrimitiveType::WChar = new Type("wchar", 2);

	const Type* const Type::PrimitiveType::Error = new Type("error-type", -1);
}