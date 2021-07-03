#include "Symple/Code/Type/Type.h"

namespace Symple::Code
{
	SYC_API const GlobalRef<const Type> Type::Default = MakeRef<const Type>(NativeType::Byte, 0);

	SYC_API const NativeType NativeType::Void = NativeType("void", NativeTypeKind::Void);
	SYC_API const NativeType NativeType::Byte = NativeType("byte", NativeTypeKind::Byte);
	SYC_API const NativeType NativeType::Short = NativeType("short", NativeTypeKind::Short);
	SYC_API const NativeType NativeType::Int = NativeType("int", NativeTypeKind::Int);
	SYC_API const NativeType NativeType::Long = NativeType("long", NativeTypeKind::Long);
	
	SYC_API const NativeType NativeType::Float = NativeType("float", NativeTypeKind::Float);
	SYC_API const NativeType NativeType::Double = NativeType("double", NativeTypeKind::Double);

	Type::Type(const TypeBase &base, uint32 ptrCount)
		: m_Base(base), m_PtrCount(ptrCount)
	{
		std::stringstream ss;
		if (m_PtrCount)
			ss << m_PtrCount;
		m_Base->Print(ss);
		m_MangledName = ss.str();

		ss.str("");
		ss << TypeKindNames[(uint32)m_Kind];
		for (uint32 i = 0; i < m_PtrCount; i++)
			ss << '*';
		m_Code = ss.str();
	}

	GlobalRef<Type> Type::Deref() const
	{
		assert("Cannot dereference a non-pointer" && m_PtrCount);
		return MakeRef<Type>(m_Kind, m_PtrCount - 1, false);
	}

	void Type::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		os << m_Code;
	}

	bool Type::Is(TypeKind kind) const
	{ return m_Kind == kind; }

	bool Type::Is(GlobalRef<const Type> ty) const
	{ return Is(ty->m_Kind) && m_PtrCount == ty->m_PtrCount; }

	TypeKind Type::GetKind() const
	{ return m_Kind; }

	uint32 Type::GetPointerCount() const
	{ return m_PtrCount; }

	bool Type::GetIsFloat() const
	{ return (m_Kind == TypeKind::Float /* || m_Kind == TypeKind::Double || m_Kind == TypeKind::Triple */) && !m_PtrCount; }

	uint32 Type::GetSize() const
	{
		if (m_PtrCount)
			return 8;

		switch (m_Kind)
		{
		case TypeKind::Void:
			return 0;
		case TypeKind::Byte:
		case TypeKind::Char:
		case TypeKind::WChar:
		case TypeKind::Bool:
			return 1;
		case TypeKind::Short:
			return 2;
		case TypeKind::Int:
		case TypeKind::Float:
			return 4;
		case TypeKind::Long:
		case TypeKind::Double:
			return 8;

		default:
			return -1;
		}
	}


	const std::string &Type::GetMangledName() const
	{ return m_MangledName; }

	const std::string &Type::GetCode() const
	{ return m_Code; }
}