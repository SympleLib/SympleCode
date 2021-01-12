#pragma once

#define nullreg (-1)
#define regax (0)
#define regsp (-2)
#define regbp (-3)

#include "SympleCode/Compiler.h"

namespace Symple
{
	class Emitter;

#if SY_32
	constexpr int NumRegisters = 4;
#else
	constexpr int NumRegisters = 12;
#endif

	typedef int Register;

	class RegisterManager
	{
	private:
		Emitter* mEmitter;

		
#if SY_32
		bool mFreeRegisters[NumRegisters] = { true, true, true, true, };
#else
		bool mFreeRegisters[NumRegisters] = { true, true, true, true, true, true,
			true, true, true, true, true, true, };
#endif

		int mSpilledRegisters[NumRegisters] = {};

#if SY_64
		static const char* const sRegisters64[NumRegisters];
#endif
		static const char* const sRegisters32[NumRegisters];
		static const char* const sRegisters16[NumRegisters];
		static const char* const sRegisters8[NumRegisters];
	public:
		RegisterManager(Emitter* emitter);

		Register Alloc(Register = nullreg);
		Register CAlloc(Register = nullreg);
		void Free(Register);
		void FreeAll();

		const bool* GetFree() const;

		static const char* GetRegister(Register reg, int size = platsize);
	};
}