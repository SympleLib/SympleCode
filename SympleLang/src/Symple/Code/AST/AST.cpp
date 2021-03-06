#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;
	using Type_t = TypeAst::Type_t;

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


	TypeAst::TypeAst(GlobalRef<const Token_t> base, const WeakTokenList &addons, GlobalRef<Type_t> ty)
		: m_Base(base), m_Addons(addons), m_Type(ty) {}

	AstKind TypeAst::GetKind() const
	{ return AstKind::Type; }

	WeakRef<const Token_t> TypeAst::GetToken() const
	{ return m_Base; }

	void TypeAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		if (!m_Base.expired())
			m_Base.lock()->Print(os, indent, label, false);
		for (auto addon : m_Addons)
			if (!addon.expired())
				addon.lock()->Print(os, indent, label, false);
		m_Type->Print(os, indent, label);
	}

	WeakRef<const Token_t> TypeAst::GetBase() const
	{ return m_Base; }

	const WeakTokenList &TypeAst::GetAddons() const
	{ return m_Addons; }

	GlobalRef<const Type_t> TypeAst::GetType() const
	{ return m_Type; }


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