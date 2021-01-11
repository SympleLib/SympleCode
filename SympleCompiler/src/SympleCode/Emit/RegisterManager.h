#pragma once

#define nullreg (-1)
#define regax (11)

namespace Symple
{
	class Emitter;

	constexpr int NumRegisters = 12;

	typedef int Register;

	class RegisterManager
	{
	private:
		Emitter* mEmitter;

		int mSpilledRegisters = 0;

		bool mFreeRegisters[NumRegisters] = { true, true, true, true, true, true,
			true, true, true, true, true, true, };

		static const char* const sRegisters64[NumRegisters];
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

		static const char* GetRegister(Register reg, int size = 4);
	};
}