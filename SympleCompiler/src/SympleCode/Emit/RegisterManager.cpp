#include "SympleCode/Emit/RegisterManager.h"

#include "SympleCode/Emit/Emitter.h"

#include "SympleCode/Analysis/Diagnostics.h"

#define Emit(fmt, ...) fprintf(mEmitter->mFile, fmt "\n", __VA_ARGS__)
#define EmitLiteral(fmt, ...) fprintf_s(mEmitter->mLiteralFile, fmt "\n", __VA_ARGS__)

namespace Symple
{
#if SY_32
	const char* const RegisterManager::sRegisters32[NumRegisters] = { "%eax", "%edx", "%ecx", "%ebx", };
	const char* const RegisterManager::sRegisters16[NumRegisters] = { "%ax", "%dx",  "%cx",  "%bx", };
	const char* const RegisterManager::sRegisters8[NumRegisters] = { "%al", "%dl",  "%cl",  "%bl", };
#else
	const char* const RegisterManager::sRegisters64[NumRegisters] = { "%rax", "%rdx", "%rcx", "%rbx", "%rdi", "%rsi",
		"%r8",  "%r9",  "%r10",  "%r11",  "%r12",  "%r13",  };
	const char* const RegisterManager::sRegisters32[NumRegisters] = { "%eax", "%edx", "%ecx", "%ebx", "%edi", "%esi",
		"%r8d", "%r9d", "%r10d", "%r11d", "%r12d", "%r13d", };
	const char* const RegisterManager::sRegisters16[NumRegisters] = { "%ax", "%dx",  "%cx",  "%bx",  "%di",  "%si" ,
		"%r8w", "%r9w", "%r10w", "%r11w", "%r12w", "%r13w", };
	const char* const RegisterManager::sRegisters8[NumRegisters]  = { "%al", "%dl",  "%cl",  "%bl",  "%dil", "%sil",
		"%r8b", "%r9b", "%r10b", "%r11b", "%r12b", "%r13b", };
#endif

	RegisterManager::RegisterManager(Emitter* emitter)
		: mEmitter(emitter) {}

	Register RegisterManager::Alloc(int regid)
	{
		if (regid != nullreg && mFreeRegisters[regid])
		{
			mFreeRegisters[regid] = false;
			return { regid };
		}

		for (int regid = 0; regid < NumRegisters; regid++)
			if (mFreeRegisters[regid])
			{
				mFreeRegisters[regid] = false;
				return { regid };
			}

		abort();
	}

	Register RegisterManager::StAlloc(int regid)
	{
		Emit("\tsub%c    $%i, %s", mEmitter->Suf(), platsize, GetRegister(regsp));
		mEmitter->mStack += platsize;
		return { nullreg, mEmitter->mStack };
	}

	Register RegisterManager::CAlloc(int regid)
	{
		Emit("\tpush%c   $0", mEmitter->Suf());
		mEmitter->mStack += platsize;
		return { nullreg, mEmitter->mStack };
	}

	void RegisterManager::Free(Register reg)
	{
		if (reg.IsStack)
			return;

		if (mFreeRegisters[reg.Id])
			abort();

		mFreeRegisters[reg.Id] = true;
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

	const char* RegisterManager::GetRegister(int regid, int sz)
	{
		if (regid == nullreg)
			return nullptr;

		if (regid == regsp)
		{
			if (sz <= 2)
				return "%sp";
			if (sz <= 4)
				return "%esp";
#if SY_64
			if (sz <= 8)
				return "%rsp";
#endif
		}

		if (regid == regbp)
		{
			if (sz <= 2)
				return "%bp";
			if (sz <= 4)
				return "%ebp";
#if SY_64
			if (sz <= 8)
				return "%rbp";
#endif
		}

		if (sz <= 1)
			return sRegisters8[regid];
		if (sz <= 2)
			return sRegisters16[regid];
		if (sz <= 4)
			return sRegisters32[regid];
#if SY_64
		if (sz <= 8)
			return sRegisters64[regid];
#endif

		return nullptr;
	}

	const char* RegisterManager::GetRegister(Register reg, int sz)
	{
		if (reg.IsStack)
		{
			char* str = new char[12];
			sprintf_s(str, 12, "-%i(%s)", reg.StackPos, GetRegister(regbp));
			return str;
		}

		return GetRegister(reg.Id);
	}
}