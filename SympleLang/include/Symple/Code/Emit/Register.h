#pragma once

namespace Symple::Code
{
	enum class SYC_API RegKind
	{
		Ax,
		Bx,
		Cx,
		Dx,

		Si,
		Di,
		R8,
		R9,
		R10,
		R11,
		R12,
		R13,
		R14,
		R15,

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
		LastX = R15, // Last `free` register
		LastXmm = Xmm7, // Last `free` float register
	};

	constexpr const char *const RegKindNames[(uint32)RegKind::Count] =
	{
		"Ax",
		"Bx",
		"Cx",
		"Dx",

		"Si",
		"Di",
		"R8",
		"R9",
		"R10",
		"R11",
		"R12",
		"R13",
		"R14",
		"R15",

		"Sp",
		"Bp",

		"Xmm0",
		"Xmm1",
		"Xmm2",
		"Xmm3",
		"Xmm4",
		"Xmm5",
		"Xmm6",
		"Xmm7",
	};

	inline std::ostream &operator <<(std::ostream &os, RegKind reg)
	{ return os << RegKindNames[(uint32)reg]; }

	constexpr const char *const Reg64[(uint32)RegKind::Count] = {
		"%rax", "%rbx", "%rcx", "%rdx",
		"%rsi", "%rdi", "%r8", "%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15",
		"%rsp", "%rbp",
	};
	constexpr const char *const Reg32[(uint32)RegKind::Count] = {
		"%eax", "%ebx", "%ecx", "%edx",
		"%esi", "%edi", "%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", "%r14d", "%r15d",
		"%esp", "%ebp",
	};
	constexpr const char *const Reg16[(uint32)RegKind::Count] = {
		"ax", "bx", "cx", "dx",
		"si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w",
		"sp", "bp",
	};
	constexpr const char *const Reg8[(uint32)RegKind::Count] = {
		"%al", "%bl", "%cl", "%dl",
		"%sil", "%dil", "%r8b", "%r9b", "%r10b", "%r11b", "%r12b", "%r13b", "%r14b", "%r15b",
		"%spl", "%bpl",
	};
}