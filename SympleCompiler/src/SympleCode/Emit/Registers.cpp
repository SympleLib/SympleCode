#include "SympleCode/Emit/Registers.h"

#include <cassert>

namespace Symple
{
	const char* Registers::AllocateRegister()
	{
		for (auto& pair : mRegisters)
			if (pair.second)
			{
				pair.second = true;
				return pair.first;
			}
	}

	void Registers::FreeRegister(const char* reg)
	{
		assert(mRegisters.find(reg) != mRegisters.end());
		mRegisters.find(reg)->second = false;
	}

	bool Registers::RegisterAvailable(const char* reg)
	{
		assert(mRegisters.find(reg) != mRegisters.end());
		return mRegisters.find(reg)->second;
	}
}