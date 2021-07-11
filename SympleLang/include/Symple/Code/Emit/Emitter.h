#pragma once

#include "Symple/Code/Ast/AST.h"

namespace llvm
{
	class LLVMContext;
	class IRBuilder;
	class Function;
}

namespace Symple::Code
{
	using namespace llvm;

	class SYC_API Emitter
	{
	private:
		Scope<LLVMContext> ctx;
		Scope<IRBuilder> builder;

		GlobalRef<const FunctionAst> m_Func;
		GlobalRef<const CompilationUnitAst> m_Unit;
	public:
		Emitter(const GlobalRef<const CompilationUnitAst> &unit);
		void Emit();
	private:
		void Emit(const GlobalRef<const FunctionAst> &function);

		void Emit(const GlobalRef<const StatementAst> &statement);
		void Emit(const GlobalRef<const ReturnStatementAst> &ret);
		void Emit(const GlobalRef<const VariableStatementAst> &var);
		void Emit(const GlobalRef<const VariableDeclarationAst> &var);

		void Emit(const GlobalRef<const ExpressionAst> &expression);
		void Emit(const GlobalRef<const PunExpressionAst> &pun); // Get it?
		void Emit(const GlobalRef<const CastExpressionAst> &cast);
		void Emit(const GlobalRef<const CallExpressionAst> &call);
		void Emit(const GlobalRef<const NameExpressionAst> &name);
		void Emit(const GlobalRef<const UnaryExpressionAst> &expression);
		void Emit(const GlobalRef<const BinaryExpressionAst> &expression);
		void Emit(const GlobalRef<const BuiltinExpressionAst> &macro);
		void Emit(const GlobalRef<const LiteralExpressionAst> &literal);

		void EmitAdr(const GlobalRef<const ExpressionAst> &expression);
		void EmitAdr(const GlobalRef<const NameExpressionAst> &name);
	private:
		uint32 Stalloc(uint32 bytes = 8);
		uint32 Staf(uint32 bytes = 8);
		uint32 Reserve(uint32 bytes = 8);

		void MovStruct(GlobalRef<const StructAst>);

		bool OverridesRegs(const GlobalRef<const Ast>);

		void Emit(const GlobalRef<const Token> &tok);
		template<typename... Args>
		void Emit(EmitKind, _Printf_format_string_ const char *fmt, Args&&... args);
		template<typename... Args>
		void Emit(_Printf_format_string_ const char *fmt, Args&&... args);

		template<typename... Args>
		void EmitDbg(EmitKind, _Printf_format_string_ const char *fmt, Args&&... args);
		template<typename... Args>
		void EmitDbg(_Printf_format_string_ const char *fmt, Args&&... args);

		char FSuf(GlobalRef<const Type>);
		constexpr char Suf(uint32 size = 8);
		constexpr const char *Reg(RegKind, uint32 size = 8);
	};
}