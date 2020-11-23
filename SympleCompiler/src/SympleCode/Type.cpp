#include "SympleCode/Type.hpp"

namespace Symple
{
	const Type Type::Void = Type{ Kinds::Void, 0, 0, true };

	const Type Type::Byte = Type{ Kinds::Byte, 1, 1, true };
	const Type Type::Short = Type{ Kinds::Short, 2, 2, true };
	const Type Type::Int = Type{ Kinds::Int, 4, 4, true };
	const Type Type::Long = Type{ Kinds::Long, 8, 8, true };

	const Type Type::SByte = Type{ Kinds::Byte, 1, 1, true };
	const Type Type::SShort = Type{ Kinds::Short, 2, 2, true };
	const Type Type::SInt = Type{ Kinds::Int, 4, 4, true };
	const Type Type::SLong = Type{ Kinds::Long, 8, 8, true };

	const Type Type::UByte = Type{ Kinds::Byte, 1, 1, false };
	const Type Type::UShort = Type{ Kinds::Short, 2, 2, false };
	const Type Type::UInt = Type{ Kinds::Int, 4, 4, false };
	const Type Type::ULong = Type{ Kinds::Long, 8, 8, false };


	const Type Type::Float = Type{ Kinds::Float, 4, 4, true };
	const Type Type::Double = Type{ Kinds::Double, 8, 8, true };

	const Type Type::Char = Type{ Kinds::Char, 1, 1, true };
	const Type Type::Bool = Type{ Kinds::Double, 1, 1, true };
}