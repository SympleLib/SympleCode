#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API Visitor {};

	class SYC_API SymbolVisitor: public Visitor
	{
	private:
		GlobalRef<CompilationUnitAst> m_Unit;
	public:
		SymbolVisitor(GlobalRef<CompilationUnitAst> compilationUnit);
		void Visit();
	};
}