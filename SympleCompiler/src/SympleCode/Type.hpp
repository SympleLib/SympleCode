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

			String,

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

		inline bool operator ==(const Type& o) const
		{
			return Kind == o.Kind && Size == o.Size && Align == o.Align && Signed == o.Signed;
		}

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

		static const Type String;

		static const Type Char;
		static const Type Bool;
	} Param;

	struct Varieble
	{
		const std::string Name;
		const Type Type;
	};

	struct Function
	{
		const std::string Name;
		const Type Type;
		const std::vector<Param> Params;
	};
}