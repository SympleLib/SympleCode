#pragma once

#include <map>

namespace Symple
{
	class Registers
	{
	private:
		std::map<const char*, bool> mRegisters = {
			{ "%eax", false },
			{ "%edx", false },
			{ "%ecx", false },
			{ "%esi", false },
			{ "%edi", false },
			{ "%ebx", false },
		};
	public:
		const char* AllocateRegister();
		void FreeRegister(const char* reg);

		bool RegisterAvailable(const char* reg);
	};
}