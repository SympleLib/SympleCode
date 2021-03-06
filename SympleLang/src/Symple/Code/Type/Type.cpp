#include "Symple/Code/Type.h"

namespace Symple::Code
{
	SYC_API std::ostream &operator <<(std::ostream &os, TypeKind kind)
	{ return os << TypeKindNames[(uint32)kind] << "Type"; }

	Type::Type(TypeKind kind, uint32 ptrCount, bool isRef)
		: m_Kind(kind), m_PtrCount(ptrCount), m_Ref(isRef)
	{}

	GlobalRef<Type> Type::Deref() const
	{ return MakeRef<Type>(m_Kind, m_PtrCount - 1, false); }

	void Type::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		os << m_Kind;
		for (uint32 i = 0; i < m_PtrCount; i++)
			os << '*';
		if (m_Ref)
			os << '&';
	}

	TypeKind Type::GetKind() const
	{ return m_Kind; }

	uint32 Type::GetPointerCount() const
	{ return m_PtrCount; }

	bool Type::GetIsRef() const
	{ return m_Ref; }
}