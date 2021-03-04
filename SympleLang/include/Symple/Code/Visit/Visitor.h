#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API Visitor;

	class SYC_API Visitor
	{
	private:
	public:
		void Visit(const Ast&);
	};
}