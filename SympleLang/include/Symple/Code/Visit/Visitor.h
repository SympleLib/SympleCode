#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API Visitor {};

	class SYC_API SymbolVisitor: public Visitor
	{
	private:
		GlobalRef<CompilationUnitAst> m_Unit;

		std::vector<uint32> m_Depths;
		std::vector<GlobalRef<const Token>> m_Names;
	public:
		SymbolVisitor(GlobalRef<CompilationUnitAst> compilationUnit);
		void Visit();
	private:
		void Visit(GlobalRef<StatementAst> statement);
		void Visit(GlobalRef<ExpressionAst> expression);
	};

	class SYC_API TypeVisitor: public Visitor
	{
	private:
		GlobalRef<FunctionAst> m_Func;
		GlobalRef<CompilationUnitAst> m_Unit;

		std::vector<uint32> m_Depths;
		std::vector<GlobalRef<const Token>> m_Names;
	public:
		TypeVisitor(GlobalRef<CompilationUnitAst> compilationUnit);
		void Visit();
	private:
		void Visit(GlobalRef<StatementAst> statement);
		void Visit(GlobalRef<ExpressionAst> expression);
	};
}