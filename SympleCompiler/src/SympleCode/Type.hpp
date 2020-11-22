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
	std::string Name;
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

	const Type* const Return;
	std::vector<Type*> Params;
	bool HasVa;
};

const Type* const VoidType = new Type{ "void" };
const Type* const IntType = new Type{ "int", sizeof(int) };