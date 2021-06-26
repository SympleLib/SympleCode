#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API CEmitter
	{
	private:
		File m_File;
		GlobalRef<const CompilationUnitAst> m_Unit;
	public:
		CEmitter(const GlobalRef<const CompilationUnitAst> &unit);
		void Emit();
	private:
		void Emit(const GlobalRef<const FunctionAst> &function);

		void Emit(const GlobalRef<const StatementAst> &statement);

		void Emit(const GlobalRef<const ExpressionAst> &expression);
		void Emit(const GlobalRef<const CallExpressionAst> &call);
	private:
		template<typename... Args>
		void Emit(_Printf_format_string_ const char *fmt, Args&&... args);
	};
}