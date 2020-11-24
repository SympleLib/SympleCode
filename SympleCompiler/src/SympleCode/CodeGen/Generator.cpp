#pragma once

#include "SympleCode/CodeGen/Generator.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <sstream>

#include "SympleCode/Util/Util.hpp"

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
			Err("Error opening file '%s': %s\n", path.c_str(), strerror(err));
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
	}

	void Push(const char* reg)
	{
		Write("push #%s", reg);
	}

	void Pop(const char* reg)
	{
		Write("pop #%s", reg);
	}

	const char* GetMovInst(long size)
	{

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

	void Set(void* data, long stackPos, long size)
	{

		long value = 0;

		Write("movl $%d %d(#rdp)", data, stackPos);
	}
}