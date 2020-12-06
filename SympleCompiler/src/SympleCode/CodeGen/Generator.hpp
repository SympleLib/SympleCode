#pragma once

#include <string>

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Tree/AST.hpp"
#include "SympleCode/Type.hpp"

#define ASM_COMMENTS true

#define ASM_ORDER_NONE  0
#define ASM_ORDER_LEFT  1
#define ASM_ORDER_RIGHT 2

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
	char* Compare(ASTToken tok);
	char* CompareNot(ASTToken tok);
	char Mod(long size);

	void WriteStr(const char* str);
	void WriteStat();

	void Push(const char* reg);
	void Pop(const char* reg);

	long Align(long size, long align);

	long Alloc(const Type& type);
	long DeAlloc(const Type& type);

	void ParseVal(const Branch& val, char str[64]);

	void BinExpr(const Branch& expr, int8_t ignore = ASM_ORDER_NONE);

	void VarDecl(const Branch& decl);
	void Assign(const Branch& expr);

	void Return(const Branch& ret);
	void StartFunc(const char* name);
	void EndFunc(const Type& ty);
	void FuncCall(const Branch& call);

	void ComVal(const Branch& val, const char* reg);
	void If(const Branch& statement);
	void Else();
	void EndIf();
}