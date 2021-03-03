#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;

	SYC_API std::ostream &operator <<(std::ostream &os, AstKind kind)
	{ return os << AstKindNames[(uint32)kind]; }


	AstKind Ast::GetKind() const
	{ return AstKind::Unknown; }

	WeakRef<const Token_t> Ast::GetToken() const
	{ return std::move(WeakRef<const Token_t>()); }


	void Ast::PrintKind(std::ostream &os) const
	{ os << Kind << "Ast"; }

	void Ast::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);
	}


	CompilationUnitAst::CompilationUnitAst(const MemberList &members, const WeakRef<const Token_t> &eof)
		: m_Members(members), m_EndOfFile(eof) {}

	AstKind CompilationUnitAst::GetKind() const
	{ return AstKind::CompilationUnit; }

	WeakRef<const Token_t> CompilationUnitAst::GetToken() const
	{ return m_EndOfFile; }


	const MemberList &CompilationUnitAst::GetMembers() const
	{ return m_Members; }

	WeakRef<const Token_t> CompilationUnitAst::GetEndOfFile() const
	{ return m_EndOfFile; }

	void CompilationUnitAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);

		for (auto member : m_Members)
			member->Print(os << '\n', indent, "", member == m_Members.back());
	}
}