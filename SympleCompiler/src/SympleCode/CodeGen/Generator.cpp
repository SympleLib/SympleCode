#pragma once

#include "SympleCode/CodeGen/Generator.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <sstream>

#include "SympleCode/Parser.hpp"

#include "SympleCode/Util/Util.hpp"
#include "SympleCode/Tree/AST.hpp"

#define Write(...) WriteF("\t" __VA_ARGS__)
#define WriteNoIndent(...) WriteF(__VA_ARGS__)

#define Pre(...) PreF(__VA_ARGS__)
#define PreIndent(...) PreF("\t" __VA_ARGS__)

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
	static FILE* sPr;
	static FILE* sIm;

	static long sStackPos;

	void Open(const std::string& path)
	{
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

		{
			errno_t err;
			if (err = fopen_s(&sPr, "asm-pre.~temp", "w+") && !sPr)
			{
				char errMsg[256];
				if (!strerror_s(errMsg, err))
					Err("Error opening PRE file: %s\n", errMsg);
				abort();
			}
		}

		{
			errno_t err;
			if (err = fopen_s(&sIm, "asm-im.~temp", "w+") && !sIm)
			{
				char errMsg[256];
				if (!strerror_s(errMsg, err))
					Err("Error opening IM file: %s\n", errMsg);
				abort();
			}
		}
	}

	void Close()
	{
		char c;
		rewind(sPr);
		rewind(sIm);
		while ((c = fgetc(sPr)) != EOF)
			fputc(c, sOut);
		while ((c = fgetc(sIm)) != EOF)
			fputc(c, sOut);

		fclose(sIm); sIm = nullptr;
		fclose(sPr); sPr = nullptr;

		remove("asm-pre.~temp");
		remove("asm-im.~temp");

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
		vfprintf(sIm, ss.str().c_str(), args);
		va_end(args);
	}

	void PreF(const char* fmt, ...)
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
		vfprintf(sPr, ss.str().c_str(), args);
		va_end(args);
	}

	char* RegDx(long size)
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

	char* RegAx(long size)
	{
		switch (size)
		{
		case 1: return "%al";
		case 2: return "%ax";
		case 4: return "%eax";
		case 8: return "%rax";
		}
		Err("Invalid size of reg ax: %d", size);
		abort();
	}

	char Mod(long size)
	{
		switch (size)
		{
		case 1: return 'b';
		case 2: return 'w';
		case 4: return 'l';
		case 8: return 'q';
		}
		Err("Invalid size of Mod: %d", size);
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

	void ParseVal(const Branch& val, char str[64])
	{
		if (val.Label == AST_CONSTANT)
		{
			snprintf(str, 64, "$%d", val.Cast<int>());
		}
		else if (val.Label == AST_VAR_VAL)
		{
			snprintf(str, 64, "-%s$(#rdp)", val.FindBranch(AST_NAME).Cast<std::string>().c_str());
		}
		else
		{
			str[0] = 'n';
			str[1] = 'u';
			str[2] = 'l';
			str[3] = 'l';
			str[4] = 0;
		}
	}

	void BinExpr(const Branch& expr)
	{
		const std::string& op = expr.FindBranch(AST_OP).Cast<std::string>();
		long size = expr.FindBranch(AST_TYPE).Cast<Type>().Size;
		char mod = Mod(size);

		const char* reg = RegAx(size);
		char lval[64], rval[64];
		ParseVal(expr.FindBranch(AST_LVALUE).Cast<Branch>(), lval);
		ParseVal(expr.FindBranch(AST_RVALUE).Cast<Branch>(), rval);
		Write("mov%c %s, %s", mod, rval, reg);
		if (op == AST_ADD)
		{
			Write("add%c %s, %s", mod, lval, reg);
		}
		else if (op == AST_SUB)
		{
			Write("sub%c %s, %s", mod, lval, reg);
		}
		else if (op == AST_MULT)
		{
			
			Write("imul%c %s, %s", mod, lval, reg);
		}
		else if (op == AST_DIV)
		{
			const char* reg = RegAx(size);
			char lval[64], rval[64];
			ParseVal(expr.FindBranch(AST_LVALUE).Cast<Branch>(), lval);
			ParseVal(expr.FindBranch(AST_RVALUE).Cast<Branch>(), rval);
			Write("sum div op with %s,%s", mod, lval, rval);
		}
	}

	void VarDecl(const Branch& decl)
	{
		std::string name = decl.FindBranch(AST_NAME).Cast<std::string>();
		long size = decl.FindBranch(AST_TYPE).Cast<Type>().Size;
		Pre("%s$ = %d ; Set stack value of %s to %d", name.c_str(),
			sStackPos += size, name.c_str(), sStackPos);
		Write("subq $%d, #rsp ; Allocate %d bytes to the stack", size, size);
		if (decl.FindBranch(AST_VALUE).Cast<Branch>().Label == AST_BIN)
		{
			BinExpr(decl.FindBranch(AST_VALUE).Cast<Branch>());
			Write("mov%c #eax, -%s$(#rdp) ; Move operation into %s", Mod(size), name.c_str(), name.c_str());
		}
		else
		{
			char val[64];
			ParseVal(decl.FindBranch(AST_VALUE).Cast<Branch>(), val);
			Write("mov%c %s, -%s$(#rdp) ; Set %s to %s", Mod(size), val, name.c_str(), name.c_str(), val);
		}
	}

	void Return(const Branch& ret)
	{
		char retv[64];
		long size = ret.FindBranch(AST_TYPE).Cast<Type>().Size;
		ParseVal(ret.FindBranch(AST_VALUE).Cast<Branch>(), retv);
		Write("mov%c %s, %s ; Return %s", Mod(size), retv, RegAx(size), retv);
	}

	void StartFunc(const char* name)
	{
		WriteNoIndent("%s: ; Declare Function", name);
		Write("; Push Stack");
		Write("pushq #rbp");
		Write("movl #rsp, #rbp");
	}

	void EndFunc(const Type& ty)
	{
		Write("popq #rbp ; Pop Stack");
		Write("ret ; Exit Function");
	}
}