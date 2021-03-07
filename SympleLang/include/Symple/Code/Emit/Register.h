#pragma once

#include "Symple/Code/Type/Type.h"

namespace Symple::Code
{
	enum class SYC_API RegKind
	{
		Ax,
		Bx,
		Cx,
		Dx,

		Sp,
		Bp,

		Count, // Count of register kinds
		LastX = Dx, // Last `free` register
	};

	constexpr const char *const RegKindNames[(uint32)RegKind::Count] =
	{
		"Ax",
		"Bx",
		"Cx",
		"Dx",

		"Sp",
		"Bp",
	};

	SYC_API std::ostream &operator <<(std::ostream &os, RegKind);

	class SYC_API Register: public Printable
	{
	public:
		using Type_t = Type;
	private:
		RegKind m_Kind;
		GlobalRef<const Type_t> m_Type;
	public:
		Register(RegKind, GlobalRef<const Type_t> type);
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		RegKind GetKind() const;
		GlobalRef<const Type_t> GetType() const;

		SY_PROPERTY_GET(GetKind) RegKind Kind;
		SY_PROPERTY_GET(GetType) GlobalRef<const Type_t> Type;
	};
}