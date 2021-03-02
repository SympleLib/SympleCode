#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	using Token_t = AST::Token_t;

	ASTKind AST::GetKind() const
	{ return ASTKind::Unknown; }

	WeakRef<const Token_t> AST::GetToken() const
	{ return std::move(WeakRef<const Token_t>()); }
}