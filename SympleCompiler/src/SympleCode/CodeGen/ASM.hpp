#pragma once

#include <string>

#include "SympleCode/Type.hpp"

#define Write(fmt, ...) WriteF(__LINE__, fmt, __VA_ARGS__)

namespace Symple::ASM
{
	void Open(const std::string& path);
	void Close();

	void WriteF(int ln, const char* fmt, ...);

	int Align(int n, int m);

	void Push(const char* reg);
	void Pop(const char* reg);

	int PushStruct(const Type& type);

	void IntCast(const Type& type);
}