#include "SympleCode/Emit/RegisterManager.h"

#include "SympleCode/Emit/Emitter.h"

#include "SympleCode/Analysis/Diagnostics.h"

#define Emit(fmt, ...) fprintf(mEmitter->mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(mEmitter->mLiteralFile, fmt "\n", __VA_ARGS__)

namespace Symple
{
	const char* const RegisterManager::sRegisters64[NumRegisters] = { "%rax", "%rdx", "%rcx", "%rbx", "%rdi", "%rsi",
		"%r8",  "%r9",  "%r10",  "%r11",  "%r12",  "%r13",  };
	const char* const RegisterManager::sRegisters32[NumRegisters] = { "%eax", "%edx", "%ecx", "%ebx", "%edi", "%esi",
		"%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", };
	const char* const RegisterManager::sRegisters16[NumRegisters] = { "%ax", "%dx",  "%cx",  "%bx",  "%di",  "%si" ,
		"%r8w", "%r9w", "%r10w", "%r11w", "%r12w", "%r13w", };
	const char* const RegisterManager::sRegisters8[NumRegisters]  = { "%al", "%dl",  "%cl",  "%bl",  "%dil", "%sil",
		"%r8b", "%r9b", "%r10b", "%r11b", "%r12b", "%r13b", };

	RegisterManager::RegisterManager(Emitter* emitter)
		: mEmitter(emitter) {}

	Register RegisterManager::Alloc(Register reg)
	{
		if (reg != nullreg)
		{
			//Emit("\t# Reserving Reg: %s (%i)", GetRegister(reg), reg);

			if (!mFreeRegisters[reg])
			{
				mSpilledRegisters[reg]++;
				mEmitter->Push(reg);
				return reg;
			}

			mFreeRegisters[reg] = false;
			return reg;
		}

		for (reg = 0; reg < NumRegisters; reg++)
			if (mFreeRegisters[reg])
			{
				mFreeRegisters[reg] = false;

				//Emit("\t# Allocated Reg: %s (%i)", GetRegister(reg), reg);
				return reg;
			}

		//Emit("\t# Spilled Reg: %s (%i)", GetRegister(reg), reg);

		mSpilledRegisters[reg]++;
		mEmitter->Push(reg);
		return reg;
	}

	Register RegisterManager::CAlloc(Register reg)
	{
		reg = Alloc(reg);
		Emit("\txor%c    %s, %s", mEmitter->Suf(), GetRegister(reg), GetRegister(reg));

		return reg;
	}

	void RegisterManager::Free(Register reg)
	{
		//Emit("\t# Free Reg: %s (%i)", GetRegister(reg), reg);

		if (reg == nullreg)
			return;

		if (mFreeRegisters[reg])
			return Diagnostics::ReportError(Token::Default, "Trying to Free Free Register");

		if (mSpilledRegisters[reg])
		{
			mSpilledRegisters[reg]--;
			mEmitter->Pop(reg);
		}
		else
			mFreeRegisters[reg] = true;
	}

	void RegisterManager::FreeAll()
	{
		for (int i = 0; i < NumRegisters; i++)
			mFreeRegisters[i] = true;
	}


	const bool* RegisterManager::GetFree() const
	{
		return mFreeRegisters;
	}

	const char* RegisterManager::GetRegister(Register reg, int sz)
	{
		if (reg == nullreg)
			return nullptr;

		if (reg == regsp)
		{
			if (sz <= 2)
				return "%sp";
			if (sz <= 4)
				return "%esp";
			if (sz <= 8)
				return "%rsp";
		}

		if (reg == regbp)
		{
			if (sz <= 2)
				return "%bp";
			if (sz <= 4)
				return "%ebp";
			if (sz <= 8)
				return "%rbp";
		}

		if (sz <= 1)
			return sRegisters8[reg];
		if (sz <= 2)
			return sRegisters16[reg];
		if (sz <= 4)
			return sRegisters32[reg];
		if (sz <= 8)
			return sRegisters64[reg];

		return nullptr;
	}
}