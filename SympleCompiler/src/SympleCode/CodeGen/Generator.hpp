#pragma once

#include <string>

#include "SympleCode/Type.hpp"

namespace Symple::ASM
{
	void Open(const std::string& path);
	void Close();

	void WriteF(const std::string& fmt, ...);

	const char* RegDx(long size);
	const char* RegAx(long size);

	void Push(const char* reg);
	void Pop(const char* reg);

	const char* GetMovInst(long size);

	long Align(long size, long align);

	long Alloc(const Type& type);
	long DeAlloc(const Type& type);

	void Set(void* data, long stackPos, long size);
}