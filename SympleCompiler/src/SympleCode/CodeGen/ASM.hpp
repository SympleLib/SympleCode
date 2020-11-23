#pragma once

#include <string>

#include "SympleCode/Type.hpp"

#define Write(fmt, ...) WriteF(__LINE__, fmt, __VA_ARGS__)

namespace Symple::ASM
{
	void Open(const std::string& path);
	void Close();

	void WriteF(int ln, const char* fmt, ...);

	const char* GetIntReg(const Type& type, char reg);
	const char* GetLoadInst(const Type& type);

	int Align(int n, int m);

	void PushXMM(int reg);
	void PopXMM(int reg);

	void Push(const char* reg);
	void Pop(const char* reg);

	int PushStruct(const Type& type);

	void MaybeWriteBitshiftLoad(const Type& type);
	void MaybeWriteBitshiftSave(const Type& type, const char* addr);

	void WriteGLoad(const Type& type, const char* label, long off);

	void WriteIntCast(const Type& type);
	void WriteToInt(const Type& type);

	void WriteLLoad(const Type& type, const char* base, int off);
}
#undef Write