#include "SympleCode/Emit/RegisterManager.h"

namespace Symple
{
	std::map<Register::TypeType, Register*> RegisterManager::mActiveRegisters;

	Register::TypeType RegisterManager::FreeRegister()
	{
		int intType = 0;
		Register::TypeType* type = (Register::TypeType*)&intType;
		while (intType > Register::Last || mActiveRegisters.find(*type) != mActiveRegisters.end())
			intType++;

		return *type;
	}

	Register* RegisterManager::Alloc()
	{
		Register::TypeType type = FreeRegister();

		Register* reg = new Register { type };
		mActiveRegisters.insert({ type, reg });
		return reg;
	}

	Register* RegisterManager::Reserve(Register::TypeType type)
	{
		if (mActiveRegisters.find(type) != mActiveRegisters.end())
		{
			Register::TypeType newType = FreeRegister();
			mActiveRegisters.at(newType) = mActiveRegisters.at(type);
			mActiveRegisters.at(newType)->Type = newType;
			mActiveRegisters.erase(mActiveRegisters.find(type));
		}

		Register* reg = new Register{ (Register::TypeType)type };
		mActiveRegisters.insert({ (Register::TypeType)type, reg });
		return reg;
	}
}