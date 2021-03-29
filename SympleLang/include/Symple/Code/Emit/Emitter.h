#pragma once

#include "Symple/Code/Ast/AST.h"
#include "Symple/Code/Emit/Register.h"

namespace Symple::Code
{
	class SYC_API Emitter
	{
	private:
		File m_File;

		uint32 m_Stack = 0, m_StackSize = 0;
		GlobalRef<const CompilationUnitAst> m_Unit;
	public:
		Emitter(const GlobalRef<const CompilationUnitAst> &unit);
		void Emit();
	private:
		void Emit(const GlobalRef<const FunctionAst> &function);

		void Emit(const GlobalRef<const StatementAst> &statement);
		void Emit(const GlobalRef<const VariableStatementAst> &var);

		void Emit(const GlobalRef<const ExpressionAst> &expression);
		void Emit(const GlobalRef<const PunExpressionAst> &pun); // Get it?
		void Emit(const GlobalRef<const CastExpressionAst> &cast);
		void Emit(const GlobalRef<const CallExpressionAst> &call);
		void Emit(const GlobalRef<const NameExpressionAst> &name);
		void Emit(const GlobalRef<const BinaryExpressionAst> &expression);
		void Emit(const GlobalRef<const LiteralExpressionAst> &literal);
	private:
		void Stalloc(uint32 bytes = 4);
		void Staf(uint32 bytes = 4);

		template<typename... Args>
		void Emit(_Printf_format_string_ const char *fmt, Args&&... args);

		constexpr const char Suf(uint32 size = 4);
		constexpr const char *Reg(RegKind, uint32 size = 4);
	};
}