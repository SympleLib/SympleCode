#pragma once

#include <string>

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Type.hpp"

#define ASM_COMMENTS true

namespace Symple::ASM
{
	void Open(const std::string& path);
	void Close();
	void WriteStandards();

	void WriteF(const char*, ...);
	void GloF(const char*, ...);
	void PreF(const char*, ...);
	void SufF(const char*, ...);

	char* RegDx(long size);
	char* RegAx(long size);
	char Mod(long size);

	void Push(const char* reg);
	void Pop(const char* reg);

	long Align(long size, long align);

	long Alloc(const Type& type);
	long DeAlloc(const Type& type);

	void ParseVal(const Branch& val, char str[64]);

	void BinExpr(const Branch& expr);

	void VarDecl(const Branch& decl);

	void Return(const Branch& ret);
	void StartFunc(const char* name);
	void EndFunc(const Type& ty);

	void FuncCall(const Branch& call);
}