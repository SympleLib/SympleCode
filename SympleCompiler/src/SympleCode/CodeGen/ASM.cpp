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

	const char* GetIntReg(const Type& type, char reg)
	{
		switch (type.Size)
		{
		case 1: return (reg == 'a') ? "al" : "cl";
		case 2: return (reg == 'a') ? "ax" : "cx";
		case 4: return (reg == 'a') ? "eax" : "ecx";
		case 8: return (reg == 'a') ? "rax" : "rcx";
		default:
			Err("Unknown data size: %s: %d", "TMNI", type.Size);
		}
	}

	const char* GetLoadInst(const Type& type)
	{
		switch (type.Size)
		{
		case 1: "movsbq";
		case 2: "movswq";
		case 4: "movslq";
		case 8: "mov";
		default:
			Err("Unknown data size for type '%s (size %i)'", "TMNI", type.Size);
		}
	}

	int Align(int n, int m)
	{
		int rem = n % m;
		return (rem == 0) ? n : n - rem + m;
	}

	void PushXMM(int reg)
	{
		Write("sub $8, #rsp");
		Write("movsd #xmm%d, (#rsp)", reg);
		sStackPos += 8;
	}

	void PopXMM(int reg)
	{
		Write("movsd (#rsp), #xmm%d", reg);
		Write("add $8, #rsp");
		sStackPos -= 8;
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

	void MaybeWriteBitshiftLoad(const Type& type)
	{
		if (type.BitSize <= 0)
			return;
		Write("shr $%d, #rax", type.BitOff);
		Push("rcx");
		Write("mov $0x%lx, #rcx", (1 << (long)type.BitSize) - 1);
		Write("and #rcx, #rax");
		Pop("rcx");
	}

	void MaybeWriteBitshiftSave(const Type& type, const char* addr)
	{
		if (type.BitSize <= 0)
			return;
		Push("rcx");
		Push("rdi");
		Write("mov $0x%lx, #rdi", (1 << (long)type.BitSize) - 1);
		Write("and #rdi, #rax");
		Write("shl $%d, #rax", type.BitOff);
		Write("mov %s, #%s", addr, GetIntReg(type, 'c'));
		Write("mov $0x%lx, #rdi", ~(((1 << (long)type.BitSize) - 1) << type.BitOff));
		Write("and #rdi, #rcx");
		Write("or #rcx, #rax");
		Pop("rdi");
		Pop("rcx");
	}

	void WriteGLoad(const Type& type, const char* label, long off)
	{
		if (type.Kind == Kinds::Array)
		{
			if (off)
				Write("lea %s+%d(#rip), #rax", label, off);
			else
				Write("lea %s(#rip), #rax", label);
		}
		const char* inst = GetLoadInst(type);
		Write("%s, %s+%d(#rip), #rax", inst, label, off);
		MaybeWriteBitshiftLoad(type);
	}

	void WriteIntCast(const Type& type)
	{
		switch (type.Kind)
		{
			using namespace Kinds;
		case Bool:
		case Char:
		case Byte:
			type.Signed ? Write("movsbq #al, #rax") : Write("movzbq #al, #rax");
			break;
		case Short:
			type.Signed ? Write("movswq #ax, #rax") : Write("movzwq #a, #rax");
			break;
		case Int:
			type.Signed ? Write("cltq") : Write("mov #eax, #eax");
			break;
		case Long:
			break;
		}
	}

	void WriteToInt(const Type& type)
	{
		if (type.Kind == Kinds::Float)
			Write("cvttss2si #xmm0, #eax");
		else if (type.Kind == Kinds::Double)
			Write("cvttss2di #xmm0, #eax");
	}

	void WriteLLoad(const Type& type, const char* base, long off)
	{
		switch (type.Kind)
		{
			using namespace Kinds;
		case Array:
			Write("lea %d(#%s), #rax", off, base);
			break;
		case Float:
			Write("movss %d(#%s), #xmm0", off, base);
			break;
		case Double:
			Write("movsd %d(#%s), #xmm0", off, base);
			break;
		default:
			const char* inst = GetLoadInst(type);
			Write("$s $d(#%s), #rax", inst, off, base);
			break;
		}
	}
}