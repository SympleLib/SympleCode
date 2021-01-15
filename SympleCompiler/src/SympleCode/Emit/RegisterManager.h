#pragma once

#define nullreg (-1)
#define regax (0)
#define regsp (-2)
#define regbp (-3)

#include "SympleCode/Compiler.h"

namespace Symple
{
	class Emitter;

	struct Register
	{
		int Id = nullreg;
		union
		{
			unsigned int StackPos = 0;
			unsigned int IsStack;
		};

		bool operator ==(const Register& other) const
		{
			return Id == other.Id && (IsStack ? StackPos == other.StackPos : true);
		}

		bool operator !=(const Register& other) const
		{
			return !operator==(other);
		}

		bool operator ==(int regid) const
		{
			return Id == regid;
		}

		bool operator !=(int regid) const
		{
			return !operator==(regid);
		}
	};

#if SY_32
	constexpr int NumRegisters = 4;
#else
	constexpr int NumRegisters = 12;
#endif

	class RegisterManager
	{
	private:
		Emitter* mEmitter;

#if SY_32
		bool mFreeRegisters[NumRegisters] = { true, true, true, true, };
#else
		bool mFreeRegisters[NumRegisters] = { true, true, true, true, true, true,
			true, true, true, true, true, true, };

		static const char* const sRegisters64[NumRegisters];
#endif
		static const char* const sRegisters32[NumRegisters];
		static const char* const sRegisters16[NumRegisters];
		static const char* const sRegisters8[NumRegisters];
	public:
		RegisterManager(Emitter* emitter);

		Register Alloc(int regid = nullreg);
		Register StAlloc(int regid = nullreg);
		Register CAlloc(int regid = nullreg);
		void Free(Register);
		void FreeAll();

		const bool* GetFree() const;

		static const char* GetRegister(int regid, int size = platsize);
		static const char* GetRegister(Register, int size = platsize);
	};
}