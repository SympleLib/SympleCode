#include "Symple/Code/AST/MemberAST.h"

namespace Symple::Code
{
	FunctionAST::FunctionAST(const GlobalRef<const Token_t> &type, const GlobalRef<const Token_t> &name,
		const WeakRef<const Token_t> &open, const WeakRef<const Token_t> &close, const GlobalRef<const StatementAST> &body)
		: m_Type(type), m_Name(name), m_Open(open), m_Close(close), m_Body(body) {}

	ASTKind FunctionAST::GetKind() const
	{ return ASTKind::Function; }
}