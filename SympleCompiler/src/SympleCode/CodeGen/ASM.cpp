#pragma once

#include "SympleCode/CodeGen/ASM.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>

#include "SympleCode/Util/Util.hpp"

namespace Symple::ASM
{
	static FILE* sOut;

	static int sStackPos;

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

	void WriteF(int line, const char* fmt, ...)
	{
		char buf[256];
		int i = 0;
		for (const char* p = fmt; *p; p++) {
			if (*p == '#') {
				buf[i++] = '%';
				buf[i++] = '%';
			}
			else {
				buf[i++] = *p;
			}
		}
		buf[i] = '\0';

		va_list args;
		va_start(args, fmt);
		vfprintf(sOut, buf, args);
		va_end(args);

		fprintf(sOut, "\n");
	}

	int Align(int n, int m)
	{
		int rem = n % m;
		return (rem == 0) ? n : n - rem + m;
	}

	void Push(const char* reg)
	{
		Write("push %s", reg);
		sStackPos += 8;
	}

	void Pop(const char* reg)
	{
		Write("pop %s", reg);
		sStackPos -= 8;\
	}

	int PushStruct(const Type& type)
	{
		int aligned = Align(type.Size, 8);
		Write("sub, $%d, #rsp", aligned);
		Write("mov #rcx, -8(#rsp)");
		Write("mov #r11, -16(#rsp)");
		Write("mov #rax, #rcx");
		int i = 0;
		for (; i < type.Size; i += 8)
		{
			Write("movq %d(#rcx), #r11", i);
			Write("mov #r11, %d(#rsp)", i);
		}
		for (; i < type.Size; i += 4)
		{
			Write("movl %d(#rcx), #r11", i);
			Write("movl #r11d, %d(#rsp)", i);
		}
		for (; i < type.Size; i += 1)
		{
			Write("movb %d(#rcx), #r11", i);
			Write("movb #r11b, %d(#rsp)", i);
		}
		Write("mov -8(#rsp), #rcx");
		Write("mov -16(#rsp), #r11");
		sStackPos += aligned;
		return aligned;
	}

	void IntCast(const Type& type)
	{
		switch (type.Name)
		{

		}
	}
}