#pragma once

#include <string>
#include <unordered_map>

namespace Symple
{
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

	typedef struct Type
	{
		Kind Kind;
		long Size;
		long Align;
		bool Signed;
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
		std::vector<Type> Params;
		bool HasVa;

		// Primitive Types
		static const Type Void;

		static const Type Byte;
		static const Type Short;
		static const Type Int;
		static const Type Long;

		static const Type SByte;
		static const Type SShort;
		static const Type SInt;
		static const Type SLong;

		static const Type UByte;
		static const Type UShort;
		static const Type UInt;
		static const Type ULong;

		static const Type Float;
		static const Type Double;

		static const Type Char;
		static const Type Bool;
	} Param, Function;
}