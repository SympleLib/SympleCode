#include "Symple/Code/Type/Type.h"

namespace Symple::Code
{
	SYC_API GlobalRef<const NativeType> NativeType::Void = GlobalRef<const NativeType>(new NativeType("void", NativeTypeKind::Void));
	SYC_API GlobalRef<const NativeType> NativeType::Byte = GlobalRef<const NativeType>(new NativeType("byte", NativeTypeKind::Byte));
	SYC_API GlobalRef<const NativeType> NativeType::Short = GlobalRef<const NativeType>(new NativeType("short", NativeTypeKind::Short));
	SYC_API GlobalRef<const NativeType> NativeType::Int = GlobalRef<const NativeType>(new NativeType("int", NativeTypeKind::Int));
	SYC_API GlobalRef<const NativeType> NativeType::Long = GlobalRef<const NativeType>(new NativeType("long", NativeTypeKind::Long));

	SYC_API GlobalRef<const NativeType> NativeType::Float = GlobalRef<const NativeType>(new NativeType("float", NativeTypeKind::Float));
	SYC_API GlobalRef<const NativeType> NativeType::Double = GlobalRef<const NativeType>(new NativeType("double", NativeTypeKind::Double));

	SYC_API GlobalRef<const NativeType> NativeType::Char = GlobalRef<const NativeType>(new NativeType("char", NativeTypeKind::Char));
	SYC_API GlobalRef<const NativeType> NativeType::WChar = GlobalRef<const NativeType>(new NativeType("wchar", NativeTypeKind::WChar));
	SYC_API GlobalRef<const NativeType> NativeType::Bool = GlobalRef<const NativeType>(new NativeType("bool", NativeTypeKind::Bool));

	SYC_API const GlobalRef<const Type> Type::Default = MakeRef<const Type>(NativeType::Byte, 0);

	Type::Type(TypeBaseRef base, uint32 ptrCount)
		: m_Base(base), m_PtrCount(ptrCount)
	{
		std::stringstream ss;
		if (m_PtrCount)
			ss << m_PtrCount;
		ss << m_Base->Name;
		m_MangledName = ss.str();

		ss.str("");
		ss << m_Base->Name;
		for (uint32 i = 0; i < m_PtrCount; i++)
			ss << '*';
		m_Code = ss.str();
	}

	GlobalRef<Type> Type::Deref() const
	{
		if (!m_PtrCount)
			throw "Cannot dereference a non-pointer";
		return MakeRef<Type>(m_Base, m_PtrCount - 1);
	}

	void Type::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		os << m_Code;
	}

	bool Type::Is(TypeBaseRef base) const
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

		auto nativeTy = Cast<const NativeType>(m_Base);
		if (!nativeTy)
			return false;

		// Hard coded for now
		switch (nativeTy->Kind)
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
		return m_Base->Size;
	}


	const std::string &Type::GetMangledName() const
	{ return m_MangledName; }

	const std::string &Type::GetCode() const
	{ return m_Code; }
}