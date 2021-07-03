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

	Type::Type(TypeBaseRef base, uint32 ptrCount)
		: m_Base(base), m_PtrCount(ptrCount)
	{
		std::stringstream ss;
		if (m_PtrCount)
			ss << m_PtrCount;
		m_Base.Print(ss);
		m_MangledName = ss.str();

		ss.str("");
		m_Base.Print(ss);
		for (uint32 i = 0; i < m_PtrCount; i++)
			ss << '*';
		m_Code = ss.str();
	}

	GlobalRef<Type> Type::Deref() const
	{
		assert("Cannot dereference a non-pointer" && m_PtrCount);
		return MakeRef<Type>(m_Base, m_PtrCount - 1);
	}

	void Type::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		os << m_Code;
	}

	bool Type::Is(const TypeBase &base) const
	{ return m_Base == base; }

	bool Type::Is(GlobalRef<const Type> ty) const
	{ return Is(ty->m_Base) && m_PtrCount == ty->m_PtrCount; }

	TypeBaseRef Type::GetBase() const
	{ return m_Base; }

	uint32 Type::GetPointerCount() const
	{ return m_PtrCount; }

	bool Type::GetIsFloat() const
	{
		if (m_PtrCount)
			return false;

		const auto &nativeTy = *dynamic_cast<const NativeType *>(&m_Base);
		if (!&nativeTy)
			return false;

		// Hard coded for now
		switch (nativeTy.Kind)
		{
		case NativeTypeKind::Float:
		case NativeTypeKind::Double:
			return true;

		default:
			return false;
		}
	}

	uint32 Type::GetSize() const
	{
		if (m_PtrCount)
			return 8;
		return m_Base.Size;
	}


	const std::string &Type::GetMangledName() const
	{ return m_MangledName; }

	const std::string &Type::GetCode() const
	{ return m_Code; }
}