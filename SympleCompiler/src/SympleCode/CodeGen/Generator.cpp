#pragma once

#include "SympleCode/CodeGen/Generator.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <sstream>

#include "SympleCode/Util/Util.hpp"
#include "SympleCode/Tree/AST.hpp"

#define Write(...) WriteF("\t" __VA_ARGS__)
#define WriteNoIndent(...) WriteF(__VA_ARGS__)

/*
* 
* rdp - stack
* rsp - stack pointer
* rax - temp storage
* rdi - this
* eax - return value, temp storage
* 
* QWORD = long , 8 bytes
* DWORD = int  , 4 bytes
* WORD  = short, 2 bytes
* BYTE  = byte , 1 byte
* 
*/

namespace Symple::ASM
{
	static FILE* sOut;

	static long sStackPos;

	void Open(const std::string& path)
	{
		errno_t err;
		if (err = fopen_s(&sOut, path.c_str(), "w") && !sOut)
		{
			char errMsg[256];
			if (!strerror_s(errMsg, err))
			Err("Error opening file '%s': %s\n", path.c_str(), errMsg);
			abort();
		}
	}

	void Close()
	{
		fclose(sOut);
	}

	void WriteF(const char* fmt, ...)
	{
		size_t fmtLen = strlen(fmt);

		std::stringstream ss;
		for (size_t i = 0; i < fmtLen; i++)
			if (fmt[i] == '#')
			{
				ss << "%%";
			}
			else
				ss << fmt[i];
		ss << '\n';

		va_list args;
		va_start(args, fmt);
		vfprintf(sOut, ss.str().c_str(), args);
		va_end(args);
	}

	const char* RegDx(long size)
	{
		switch (size)
		{
		case 1: return "%dl";
		case 2: return "%dx";
		case 4: return "edx";
		case 8: return "rdx";
		}
		Err("Invalid size of reg dx: %d", size);
		abort();
	}

	const char* RegAx(long size)
	{
		switch (size)
		{
		case 1: return "%al";
		case 2: return "%ax";
		case 4: return "eax";
		case 8: return "rax";
		}
		Err("Invalid size of reg ax: %d", size);
		abort();
	}

	void Push(const char* reg)
	{
		Write("push #%s", reg);
	}

	void Pop(const char* reg)
	{
		Write("pop #%s", reg);
	}

	long Align(long size, long align)
	{ return (size + align - 1) / align * align; }

	long Alloc(const Type& ty)
	{
		long size = Align(ty.Size, 8);
		Write("sub $%d, #rsp", size);
		return sStackPos -= size;
	}

	long DeAlloc(const Type& ty)
	{
		long size = Align(ty.Size, 8);
		Write("add $%d, #rsp", size);
		return sStackPos += size;
	}

	void BinExpr(const Branch& expr)
	{
		bool islConst = std::any_cast<Branch>(expr.FindBranch(AST_LVALUE).Data).Label == AST_CONSTANT;
		bool isrConst = std::any_cast<Branch>(expr.FindBranch(AST_RVALUE).Data).Label == AST_CONSTANT;

		Out("l: %s, r: %s", islConst ? "true" : "false", isrConst ? "true" : "false");

		const std::string& op = std::any_cast<std::string>(expr.FindBranch(AST_BIN).Data);
		if (op == AST_ADD && islConst && isrConst)
		{
			Write("movl $%d, #eax", std::any_cast<int>(std::any_cast<Branch>(expr.FindBranch(AST_RVALUE).Data).Data));
			Write("addl $%d, #eax", std::any_cast<int>(std::any_cast<Branch>(expr.FindBranch(AST_LVALUE).Data).Data));
		}
		else if (op == AST_SUB && islConst && isrConst)
		{
			Write("movl $%d, #eax", std::any_cast<int>(std::any_cast<Branch>(expr.FindBranch(AST_RVALUE).Data).Data));
			Write("subl $%d, #eax", std::any_cast<int>(std::any_cast<Branch>(expr.FindBranch(AST_LVALUE).Data).Data));
		}
		else if (op == AST_MULT && islConst && isrConst)
		{
			Write("movl $%d, #eax", std::any_cast<int>(std::any_cast<Branch>(expr.FindBranch(AST_RVALUE).Data).Data));
			Write("imull $%d, #eax", std::any_cast<int>(std::any_cast<Branch>(expr.FindBranch(AST_LVALUE).Data).Data));
		}
		else if (op == AST_DIV && islConst && isrConst)
		{
			Write("movl $%d, #eax", std::any_cast<int>(std::any_cast<Branch>(expr.FindBranch(AST_RVALUE).Data).Data));
			Write("cltd");
			Write("idivl $%d", std::any_cast<int>(std::any_cast<Branch>(expr.FindBranch(AST_LVALUE).Data).Data));
		}
	}
}