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

	Register RegisterManager::Alloc(AsmRegister asmreg)
	{
		Register reg = nullptr;

		for (asmreg = 0; asmreg < NumRegisters; asmreg++)
			if (mFreeRegisters[asmreg])
			{
				//Emit("\t# Alloc Reg: %s (%i)", GetRegister(asmreg), asmreg);
				mFreeRegisters[asmreg] = false;
				reg = new _Register { asmreg };
				goto Return;
			}

		for (asmreg = 0; asmreg < NumRegisters; asmreg++)
			if (mRegisters[asmreg] > mRegisters[(asmreg + 1) % NumRegisters])
			{
				asmreg++;
				break;
			}
		if (asmreg >= NumRegisters)
			asmreg = 0;

		mEmitter->mStack += platsize;
		mRegisters[asmreg].back()->IsSpilled = mEmitter->mStack;
		mEmitter->Push(mRegisters[asmreg].back());
		reg = new _Register{ asmreg };
		goto Return;
		
	Return:
		mRegisters[asmreg].push_back(reg);
		return reg;
	}

	Register RegisterManager::CAlloc(AsmRegister asmreg)
	{
		Register reg = Alloc(asmreg);
		Emit("\txor%c    %s, %s", mEmitter->Suf(), GetRegister(reg), GetRegister(reg));

		return reg;
	}

	void RegisterManager::Free(Register reg)
	{
		Emit("\t# Free Reg: %s (%i)", GetRegister(reg->AsmRegister), reg->AsmRegister);

		if (reg->AsmRegister == nullreg)
		{
			Emit("\t# Trying to Free Null Register");
			return Diagnostics::ReportError(Token::Default, " Trying to Free Null Register");
		}

		if (mFreeRegisters[reg->AsmRegister])
		{
			Emit("\t# Trying to Free Free Register: %s (%i)", GetRegister(reg->AsmRegister), reg->AsmRegister);
			return Diagnostics::ReportError(Token::Default, "Trying to Free Free Register: %s (%i)", GetRegister(reg->AsmRegister), reg->AsmRegister);
		}

		if (mRegisters[reg->AsmRegister].size() > 1 && mRegisters[reg->AsmRegister][mRegisters[reg->AsmRegister].size() - 2]->IsSpilled)
		{
			mRegisters[reg->AsmRegister][mRegisters[reg->AsmRegister].size() - 2]->IsSpilled = false;
		}
		else
			mFreeRegisters[reg->AsmRegister] = true;

		mRegisters[reg->AsmRegister].pop_back();
	}

	void RegisterManager::FreeAll()
	{
		
	}


	const bool* RegisterManager::GetFree() const
	{
		return mFreeRegisters;
	}

	const char* RegisterManager::GetRegister(AsmRegister reg, int sz)
	{
		if (reg == nullreg)
			return nullptr;

		if (reg == regsp)
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

		if (reg == regbp)
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
			return sRegisters8[reg];
		if (sz <= 2)
			return sRegisters16[reg];
		if (sz <= 4)
			return sRegisters32[reg];
#if SY_64
		if (sz <= 8)
			return sRegisters64[reg];
#endif

		return nullptr;
	}

	const char* RegisterManager::GetRegister(Register reg, int sz)
	{
		if (reg->IsSpilled)
		{
			char* str = new char[12];
			sprintf_s(str, 12, "-%i(%s)", reg->StackPos, GetRegister(regbp));
			return str;
		}

		return GetRegister(reg->AsmRegister, sz);
	}
}