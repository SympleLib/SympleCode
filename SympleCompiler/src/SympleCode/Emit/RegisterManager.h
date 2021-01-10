#pragma once

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

		Register Alloc();
		void Free(Register);
		void FreeAll();

		static const char* GetRegister(Register reg, int size = 4);
	};
}