#pragma once

#include "Symple/Code/AST/AST.h"
#include "Symple/Code/Visit/VisitMe.h"
#include "Symple/Code/BugCheck/ErrorList.h"

namespace Symple::Code
{
	class SYC_API Visitor {};

	class SYC_API SymbolVisitor: public Visitor
	{
	private:
		ErrorList *m_ErrorList;
		GlobalRef<CompilationUnitAst> m_Unit;

		std::vector<uint32> m_Depths;
		std::vector<GlobalRef<const Symbol>> m_Names;
	public:
		SymbolVisitor(GlobalRef<CompilationUnitAst>);
		void Visit(ErrorList *);
	private:
		void Mangle(GlobalRef<Function>);
		void Mangle(GlobalRef<ParameterAst>);
		void Mangle(GlobalRef<VariableStatementAst>);

		void Visit(GlobalRef<StatementAst>);
		void Visit(GlobalRef<ExpressionAst>);
	};

	class SYC_API TypeVisitor: public Visitor
	{
	private:
		ErrorList *m_ErrorList;
		GlobalRef<FunctionAst> m_Func;
		GlobalRef<CompilationUnitAst> m_Unit;
	public:
		TypeVisitor(GlobalRef<CompilationUnitAst>);
		void Visit(ErrorList *);
	private:
		void Visit(GlobalRef<StatementAst>);
		void Visit(GlobalRef<ExpressionAst>);
	};
}