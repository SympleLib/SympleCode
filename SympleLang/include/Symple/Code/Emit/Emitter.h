#pragma once

#include "Symple/Code/Ast/AST.h"
#include "Symple/Code/Emit/Register.h"

namespace Symple::Code
{
	class SYC_API Emitter
	{
	private:
		File m_File;

		GlobalRef<const CompilationUnitAst> m_Unit;
	public:
		Emitter(const GlobalRef<const CompilationUnitAst> &unit);
		void Emit();
	private:
		void Emit(const GlobalRef<const FunctionAst> &function);
	private:
		template<typename... Args>
		void Emit(const char *fmt, Args&&... args);

		const char *Reg(RegKind, uint32 size = 4);
	};
}