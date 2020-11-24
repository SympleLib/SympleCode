#pragma once

#include <string>

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Type.hpp"

namespace Symple::ASM
{
	struct Value
	{

	};

	void Open(const std::string& path);
	void Close();

	void WriteF(const char*, ...);

	const char* RegDx(long size);
	const char* RegAx(long size);

	void Push(const char* reg);
	void Pop(const char* reg);

	long Align(long size, long align);

	long Alloc(const Type& type);
	long DeAlloc(const Type& type);

	void BinExpr(const Branch& expr);
}