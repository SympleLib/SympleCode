#include "Symple/Code/Emit/Register.h"

namespace Symple::Code
{
	using Type_t = Register::Type_t;

	SYC_API std::ostream &operator <<(std::ostream &os, RegKind kind)
	{ return os << "Reg" << RegKindNames[(uint32)kind]; }

	Register::Register(RegKind kind, GlobalRef<const Type_t> ty)
		: m_Kind(kind), m_Type(ty) {}

	void Register::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os);
		os << m_Kind;
		
		indent += GetAddIndent(last);
		m_Type->Print(os, indent, "Type = ");
	}

	RegKind Register::GetKind() const
	{ return m_Kind; }

	GlobalRef<const Type_t> Register::GetType() const
	{ return m_Type; }
}