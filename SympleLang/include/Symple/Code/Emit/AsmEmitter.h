#pragma once

#include "Symple/Code/Ast/AST.h"
#include "Symple/Code/Emit/Register.h"

namespace Symple::Code
{
	enum class SYC_API EmitKind
	{
		Text,
		Data,
	};

	class SYC_API AsmEmitter
	{
	private:
		File m_TextFile, m_DataFile;
		GlobalRef<const FunctionAst> m_Func;

		// File num, Func id
		uint32 m_FNum = 0, m_FId = 0;

		uint32 m_Label = 0;
		uint32 m_Stack = 0, m_StackSize = 0;
		GlobalRef<const CompilationUnitAst> m_Unit;
	public:
		AsmEmitter(const GlobalRef<const CompilationUnitAst> &unit);
		void Emit();
	private:
		void Emit(const GlobalRef<const FunctionAst> &function);

		void Emit(const GlobalRef<const StatementAst> &statement);
		void Emit(const GlobalRef<const ReturnStatementAst> &ret);
		void Emit(const GlobalRef<const VariableStatementAst> &var);

		void Emit(const GlobalRef<const ExpressionAst> &expression);
		void Emit(const GlobalRef<const PunExpressionAst> &pun); // Get it?
		void Emit(const GlobalRef<const CastExpressionAst> &cast);
		void Emit(const GlobalRef<const CallExpressionAst> &call);
		void Emit(const GlobalRef<const NameExpressionAst> &name);
		void Emit(const GlobalRef<const UnaryExpressionAst> &expression);
		void Emit(const GlobalRef<const BinaryExpressionAst> &expression);
		void Emit(const GlobalRef<const BuiltinExpressionAst> &macro);
		void Emit(const GlobalRef<const LiteralExpressionAst> &literal);
	private:
		void Stalloc(uint32 bytes = 4);
		void Staf(uint32 bytes = 4);

		void Emit(const GlobalRef<const Token> &tok);
		template<typename... Args>
		void Emit(EmitKind, _Printf_format_string_ const char *fmt, Args&&... args);
		template<typename... Args>
		void Emit(_Printf_format_string_ const char *fmt, Args&&... args);

		template<typename... Args>
		void EmitDbg(EmitKind, _Printf_format_string_ const char *fmt, Args&&... args);
		template<typename... Args>
		void EmitDbg(_Printf_format_string_ const char *fmt, Args&&... args);

		constexpr const char Suf(uint32 size = 4);
		constexpr const char *Reg(RegKind, uint32 size = 4);
	};
}