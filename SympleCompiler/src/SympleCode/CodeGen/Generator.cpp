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

#define Glo(...) GloF(__VA_ARGS__)
#define GloIndent(...) GloF("\t" __VA_ARGS__)

#define Pre(...) PreF(__VA_ARGS__)
#define PreIndent(...) PreF("\t" __VA_ARGS__)

#define Suf(...) SufF("\t" __VA_ARGS__)
#define SufNoIndent(...) SufF(__VA_ARGS__)

/*
* 
* rbp - stack
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
	static FILE* sGl;
	static FILE* sSu;
	static FILE* sIm;

	static long sStackPos;
	static long sDatPos;

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
			if (err = fopen_s(&sGl, "asm-glo.~temp", "w+") && !sGl)
			{
				char errMsg[256];
				if (!strerror_s(errMsg, err))
					Err("Error opening GLO file: %s\n", errMsg);
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
			if (err = fopen_s(&sSu, "asm-suf.~temp", "w+") && !sSu)
			{
				char errMsg[256];
				if (!strerror_s(errMsg, err))
					Err("Error opening SUF file: %s\n", errMsg);
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
		rewind(sGl);
		rewind(sSu);
		rewind(sIm);
		while ((c = fgetc(sGl)) != EOF)
			fputc(c, sOut);
		while ((c = fgetc(sPr)) != EOF)
			fputc(c, sOut);
		while ((c = fgetc(sIm)) != EOF)
			fputc(c, sOut);
		while ((c = fgetc(sSu)) != EOF)
			fputc(c, sOut);

		fclose(sIm); sIm = nullptr;
		fclose(sSu); sSu = nullptr;
		fclose(sPr); sPr = nullptr;
		fclose(sGl); sGl = nullptr;

		remove("asm-im.~temp");
		remove("asm-glo.~temp");
		remove("asm-pre.~temp");
		remove("asm-suf.~temp");

		fclose(sOut);
	}
	
	void WriteStandards()
	{
		Glo(".global print");
		WriteNoIndent("print:");
		Write("movq -message$(#rbp), #rcx");

		Write("subq $72, #rsp");
		Write("movq #r9, 104(#rsp)");
		Write("movq	#r8, 96(#rsp)");
		Write("movq	#rdx, 88(#rsp)");
		Write("movq	#rcx, 64(#rsp)");
		Write("leaq	88(#rsp), #rax");
		Write("leaq	88(#rsp), #rax");
		Write("movq	#rax, 48(#rsp)");
		Write("movq	48(#rsp), #r9");
		Write("movq	64(#rsp), #rdx");
		Write("movl	$1, #ecx");
		Write("movq	#r9, 40(#rsp)");
		Write("movq	#rdx, 32(#rsp)");
		Write("callq __acrt_iob_func");
		Write("xorl	#ecx, #ecx");
		Write("movl	#ecx, #r8d");
		Write("movq	#rax, #rcx");
		Write("movq	32(#rsp), #rdx");
		Write("movq	40(#rsp), #r9");
		Write("callq _vfprintf_l");
		Write("movl #eax, 60(#rsp)");
		Write("movl 60(#rsp), #eax");
		Write("addq $72, #rsp");
		Write("retq");
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
			else if (fmt[i] == ';')
			{
#if ASM_COMMENTS
				ss << '#';
#else
				while (i < fmtLen && fmt[i] != '\n')
					i++;
#endif
			}
			else
				ss << fmt[i];
		ss << '\n';

		va_list args;
		va_start(args, fmt);
		vfprintf(sIm, ss.str().c_str(), args);
		va_end(args);
	}

	void GloF(const char* fmt, ...)
	{
		size_t fmtLen = strlen(fmt);

		std::stringstream ss;
		for (size_t i = 0; i < fmtLen; i++)
			if (fmt[i] == '#')
			{
				ss << "%%";
			}
			else if (fmt[i] == ';')
			{
#if ASM_COMMENTS
				ss << '#';
#else
				while (i < fmtLen && fmt[i] != '\n')
					i++;
#endif
			}
			else
				ss << fmt[i];
		ss << '\n';

		va_list args;
		va_start(args, fmt);
		vfprintf(sGl, ss.str().c_str(), args);
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
			else if (fmt[i] == ';')
			{
#if ASM_COMMENTS
				ss << '#';
#else
				while (i < fmtLen && fmt[i] != '\n')
					i++;
#endif
			}
			else
				ss << fmt[i];
		ss << '\n';

		va_list args;
		va_start(args, fmt);
		vfprintf(sPr, ss.str().c_str(), args);
		va_end(args);
	}

	void SufF(const char* fmt, ...)
	{
		size_t fmtLen = strlen(fmt);

		std::stringstream ss;
		for (size_t i = 0; i < fmtLen; i++)
			if (fmt[i] == '#')
			{
				ss << "%%";
			}
			else if (fmt[i] == ';')
			{
#if ASM_COMMENTS
				ss << '#';
#else
				while (i < fmtLen && fmt[i] != '\n')
					i++;
#endif
			}
			else
				ss << fmt[i];
		ss << '\n';

		va_list args;
		va_start(args, fmt);
		vfprintf(sSu, ss.str().c_str(), args);
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
			snprintf(str, 64, "-%s$(%%rbp)", val.FindBranch(AST_NAME).Cast<std::string>().c_str());
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

	void BinExpr(const Branch& expr, int8_t ignore)
	{
		const std::string& op = expr.FindBranch(AST_OP).Cast<std::string>();
		long size = expr.FindBranch(AST_TYPE).Cast<Type>().Size;
		char mod = Mod(size);

		const char* reg = RegAx(size);

		char lval[64], rval[64];
		ParseVal(expr.FindBranch(AST_LVALUE).Cast<Branch>(), lval);
		ParseVal(expr.FindBranch(AST_RVALUE).Cast<Branch>(), rval);

		if (expr.FindBranch(AST_LVALUE).Cast<Branch>().Label == AST_BIN)
		{
			BinExpr(expr.FindBranch(AST_LVALUE).Cast<Branch>(), ASM_ORDER_LEFT);
			if (expr.FindBranch(AST_RVALUE).Cast<Branch>().Label == AST_BIN)
			{
				BinExpr(expr.FindBranch(AST_RVALUE).Cast<Branch>(), ASM_ORDER_RIGHT);
				return;
			}
			else
				goto PrintR;
		}
		else if (expr.FindBranch(AST_RVALUE).Cast<Branch>().Label == AST_BIN)
		{
			BinExpr(expr.FindBranch(AST_RVALUE).Cast<Branch>(), ASM_ORDER_RIGHT);
			goto PrintL;
		}
		else
		{
			Write("mov%c %s, %s", mod, lval, reg);
			goto PrintR;
		}
		return;
	PrintL:
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
			Write("sum div op with %s,%s", mod, lval, rval);
		}
		return;
	PrintR:
		if (op == AST_ADD)
		{
			Write("add%c %s, %s", mod, rval, reg);
		}
		else if (op == AST_SUB)
		{
			Write("sub%c %s, %s", mod, rval, reg);
		}
		else if (op == AST_MULT)
		{

			Write("imul%c %s, %s", mod, rval, reg);
		}
		else if (op == AST_DIV)
		{
			Write("sum div op with %s,%s", mod, rval, rval);
		}
		return;
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
			Write("mov%c #eax, -%s$(#rbp) ; Move operation into %s", Mod(size), name.c_str(), name.c_str());
		}
		else if (decl.FindBranch(AST_VALUE).Cast<Branch>().Label == AST_STRING)
		{
			Glo(".global _Dat$%d", ++sDatPos);
			SufNoIndent("_Dat$%d:", sDatPos);
			Suf(".asciz \"%s\"", decl.FindBranch(AST_VALUE).Cast<Branch>().Cast<std::string>().c_str());

			Write("leaq _Dat$%d(#rip), #rcx", sDatPos);
			Write("movq #rcx, -%s$(#rbp)", name.c_str());
		}
		else
		{
			if (decl.FindBranch(AST_VALUE).Cast<Branch>().Label != AST_VAR_VAL)
			{
				char val[64];
				ParseVal(decl.FindBranch(AST_VALUE).Cast<Branch>(), val);
				Write("mov%c %s, -%s$(#rbp) ; Set %s to %s", Mod(size), val, name.c_str(), name.c_str(), val);
			}
			else
			{
				char val[64];
				ParseVal(decl.FindBranch(AST_VALUE).Cast<Branch>(), val);
				long rsize = decl.FindBranch(AST_VALUE).Cast<Branch>().FindBranch(AST_TYPE).Cast<Type>().Size;
				Write("; Set %s to %s", name.c_str(), val);
				Write("mov%c %s, %s", Mod(rsize), val, RegAx(size));
				Write("mov%c %s, -%s$(#rbp)", Mod(size), RegAx(size), name.c_str());
			}
		}
	}

	void Assign(const Branch& expr)
	{
		const Varieble* left = nullptr;
		std::string name = expr.FindBranch(AST_LVALUE).Cast<Branch>().FindBranch(AST_NAME).Cast<std::string>();
		for (const auto& var : Parser::sVars)
			if (var.Name == name)
				left = &var;
		if (!left)
		{
			Err("Internal Error!");
			abort();
		}
		long size = left->Type.Size;
		if (expr.FindBranch(AST_RVALUE).Cast<Branch>().Label == AST_BIN)
		{
			BinExpr(expr.FindBranch(AST_RVALUE).Cast<Branch>());
			const char* reg = RegAx(size);
			Write("mov%c %s, -%s$(#rbp) ; Move operation into %s", Mod(size), reg, name.c_str(), name.c_str());
		}
		else if (expr.FindBranch(AST_RVALUE).Cast<Branch>().Label == AST_STRING)
		{
			Glo(".global _Dat$%d", ++sDatPos);
			SufNoIndent("_Dat$%d:", sDatPos);
			Suf(".asciz \"%s\"", expr.FindBranch(AST_RVALUE).Cast<Branch>().Cast<std::string>().c_str());

			Write("leaq _Dat$%d(#rip), #rcx", sDatPos);
			Write("movq #rcx, -%s$(#rbp)", name.c_str());
		}
		else
		{
			if (expr.FindBranch(AST_RVALUE).Cast<Branch>().Label != AST_VAR_VAL)
			{
				char val[64];
				ParseVal(expr.FindBranch(AST_RVALUE).Cast<Branch>(), val);
				Write("mov%c %s, -%s$(#rbp) ; Set %s to %s", Mod(size), val, name.c_str(), name.c_str(), val);
			}
			else
			{
				char val[64];
				ParseVal(expr.FindBranch(AST_RVALUE).Cast<Branch>(), val);
				long size = expr.FindBranch(AST_RVALUE).Cast<Branch>().FindBranch(AST_TYPE).Cast<Type>().Size;
				Write("; Set %s to %s", name.c_str(), val);
				Write("mov%c %s, %s", Mod(size), val, RegAx(size));
				Write("mov%c %s, -%s$(#rbp)", Mod(size), RegAx(size), name.c_str());
			}
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
		Glo(".global %s", name);
		WriteNoIndent("%s: ; Declare Function", name);
		Write("; Push Stack");
		Write("pushq #rbp");
		Write("mov #rsp, #rbp");
	}

	void EndFunc(const Type& ty)
	{
		Write("popq #rbp ; Pop Stack");
		Write("ret ; Exit Function");
	}

	void FuncCall(const Branch& call)
	{
		Write("callq %s", call.FindBranch(AST_NAME).Cast<std::string>().c_str());
	}
}