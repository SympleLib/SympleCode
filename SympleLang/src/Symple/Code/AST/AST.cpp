#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	using Token_t = AST::Token_t;

	ASTKind AST::GetKind() const
	{ return ASTKind::Unknown; }

	WeakRef<const Token_t> AST::GetToken() const
	{ return std::move(WeakRef<const Token_t>()); }


	CompilationUnitAST::CompilationUnitAST(const MemberList &members, const WeakRef<const Token_t> &eof)
		: m_Members(members), m_EndOfFile(eof) {}

	ASTKind CompilationUnitAST::GetKind() const
	{ return ASTKind::CompilationUnit; }

	WeakRef<const Token_t> CompilationUnitAST::GetToken() const
	{ return m_EndOfFile; }


	const MemberList &CompilationUnitAST::GetMembers() const
	{ return m_Members; }

	WeakRef<const Token_t> CompilationUnitAST::GetEndOfFile() const
	{ return m_EndOfFile; }
}