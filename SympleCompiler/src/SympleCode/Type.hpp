#pragma once

#include <string>
#include <unordered_map>

using Kind = int8_t;

namespace Kinds
{
	typedef enum : Kind
	{
		Void,
		Byte,
		Short,
		Int,
		Long,

		Float,
		Double,

		Char,
		Bool,

		Array,
		Pointer,

		Enum,
		Struct,
		Interface,
		Impl,
		Class,

		Func
	} kind;
}

struct Type
{
	Kind Name;
	long Size;
	long Align;
	bool Signed = true;
	bool Static;

	Type* Ptr; // Pointer
	int Length; // Array Length

	std::vector<Type> Fields; // Struct Fields
	long Offset; // Struct Offset
	bool IsStruct;

	// Bit fields
	long BitOff;
	long BitSize;
	// Function Data
	const Type* const Return;
	std::vector<Type*> Params;
	bool HasVa;

	static const Type Void;
	static const Type Byte;
	static const Type Short;
	static const Type Int;
};

const Type Type::Void = Type{ Kinds::Void, 0, 0, true };
const Type Type::Byte = Type{ Kinds::Byte, 1, 1, true };
const Type Type::Short = Type{ Kinds::Short, 2, 2, true };
const Type Type::Int = Type{ Kinds::Int, 4, 4, true };