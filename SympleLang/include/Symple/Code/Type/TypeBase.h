#pragma once

#include "Symple/Code/Common.h"
#include "Symple/Code/Util/Printable.h"

namespace Symple::Code
{
	class SYC_API TypeBase: public Printable
	{
	protected:
		std::string m_Name;
	public:
		TypeBase(std::string_view name)
			: m_Name(name) {}

		bool operator ==(const TypeBase &other) const
		{ return this == &other; }
		virtual uint32 GetSize() const = 0;
		const std::string &GetName() const
		{ return m_Name; }

		SY_PROPERTY_GET(GetName) const std::string &Name;
		SY_PROPERTY_GET(GetSize) uint32 Size;
	};

	using TypeBaseRef = const TypeBase &;

	enum class SYC_API NativeTypeKind
	{
		Void,
		Byte,
		Short,
		Int,
		Long,

		Float,
		Double,

		Char,
		WChar,
		Bool,

		Count, // Count of type kinds
	};

	constexpr const char *const NativeTypeNames[(uint32)NativeTypeKind::Count] = {
		"Void",
		"Byte",
		"Short",
		"Int",
		"Long",

		"Float",
		"Double",

		"Char",
		"WChar",
		"Bool",
	};

	constexpr const uint32 NativeTypeSizes[(uint32)NativeTypeKind::Count] = {
		0,
		1,
		2,
		4,
		8,

		4,
		8,

		1,
		4,
		1,
	};
	
	class SYC_API NativeType: public TypeBase
	{
	private:
		NativeTypeKind m_Kind;

		NativeType(std::string_view name, NativeTypeKind kind)
			: TypeBase(name), m_Kind(kind) {}
	public:
		static const NativeType Void;
		static const NativeType Byte;
		static const NativeType Short;
		static const NativeType Int;
		static const NativeType Long;
		static const NativeType Float;
		static const NativeType Double;
		static const NativeType Char;
		static const NativeType WChar;
		static const NativeType Bool;
	public:
		virtual uint32 GetSize() const override
		{ return NativeTypeSizes[(uint32)m_Kind]; }
		NativeTypeKind GetKind() const
		{ return m_Kind; }

		SY_PROPERTY_GET(GetKind) NativeTypeKind Kind;
	};
}