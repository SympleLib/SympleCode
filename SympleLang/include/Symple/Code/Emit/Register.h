#pragma once

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

		Xmm0,
		Xmm1,
		Xmm2,
		Xmm3,
		Xmm4,
		Xmm5,
		Xmm6,
		Xmm7,

		Count, // Count of register kinds
		LastX = Dx, // Last `free` register
		LastXmm = Xmm7, // Last `free` float register
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

	inline std::ostream &operator <<(std::ostream &os, RegKind reg)
	{ return os << RegKindNames[(uint32)reg]; }
}