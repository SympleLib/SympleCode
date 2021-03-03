#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	using Token_t = AST::Token_t;

	ASTKind AST::GetKind() const
	{ return ASTKind::Unknown; }

	WeakRef<const Token_t> AST::GetToken() const
	{ return std::move(WeakRef<const Token_t>()); }


	void AST::PrintKind(std::ostream &os) const
	{ os << ASTKindNames[(uint32)Kind] << "AST"; }

	void AST::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);
	}


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

	void CompilationUnitAST::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);

		for (auto member : m_Members)
			member->Print(os << '\n', indent, "", member == m_Members.back());
	}
}