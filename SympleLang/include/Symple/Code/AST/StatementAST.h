#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API StatementAST: public AST
	{
	public:
		virtual ASTKind GetKind() const override;
	};
}