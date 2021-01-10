#include "SympleCode/Emit/RegisterManager.h"

#include "SympleCode/Emit/Emitter.h"

#include "SympleCode/Analysis/Diagnostics.h"

#define Emit(fmt, ...) fprintf(Emiter::mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(Emiter::mLiteralFile, fmt "\n", __VA_ARGS__)

namespace Symple
{
	const char* const RegisterManager::sRegisters64[NumRegisters] = { "%rax", "%rdx", "%rcx", "%rbx", "%rdi", "%rsi",
			"%r8", "%r9", "%r10", "%r11", "%r12", "%r13", };
	const char* const RegisterManager::sRegisters32[NumRegisters] = { "%eax", "%edx", "%ecx", "%ebx", "%edi", "%esi",
		"%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", };
	const char* const RegisterManager::sRegisters16[NumRegisters] = { "%ax",  "%dx",  "%cx",  "%bx",  "%di",  "%si",
		"%r8w", "%r9w", "%r10w", "%r11w", "%r12w", "%r13w", };
	const char* const RegisterManager::sRegisters8[NumRegisters] = { "%al",  "%dl",  "%cl",  "%bl",  "%dil", "%sil",
		"%r8b", "%r9b", "%r10b", "%r11b", "%r12b", "%r13b", };

	RegisterManager::RegisterManager(Emitter* emitter)
		: mEmitter(emitter) {}

	Register RegisterManager::Alloc()
	{
		int reg;
		for (reg = 0; reg < NumRegisters; reg++)
			if (mFreeRegisters[reg])
			{
				mFreeRegisters[reg] = false;
				return reg;
			}

		reg = mSpilledRegisters % NumRegisters;
		mSpilledRegisters++;

		mEmitter->Push(reg);
		return reg;
	}

	void RegisterManager::Free(Register reg)
	{
		if (reg == nullreg)
			return;

		if (mFreeRegisters[reg])
			return Diagnostics::ReportError(Token::Default, "Trying to Free Free Register");

		if (mSpilledRegisters)
		{
			mSpilledRegisters--;
			reg = mSpilledRegisters % NumRegisters;

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

	const char* RegisterManager::GetRegister(Register reg, int sz)
	{
		if (reg == nullreg)
			return nullptr;

		switch (sz)
		{
		case 1:
			return sRegisters8[reg];
		case 2:
			return sRegisters16[reg];
		case 4:
			return sRegisters32[reg];
		case 8:
			return sRegisters64[reg];
		}

		return nullptr;
	}
}