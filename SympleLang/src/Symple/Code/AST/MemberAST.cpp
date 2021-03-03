#include "Symple/Code/AST/MemberAST.h"

namespace Symple::Code
{
	using Token_t = AST::Token_t;

	ASTKind MemberAST::GetKind() const
	{ return ASTKind::Member; }


	FunctionAST::FunctionAST(const GlobalRef<const Token_t> &type, const GlobalRef<const Token_t> &name,
		const WeakRef<const Token_t> &open, const WeakRef<const Token_t> &close, const GlobalRef<const StatementAST> &body)
		: m_Type(type), m_Name(name), m_Open(open), m_Close(close), m_Body(body) {}

	ASTKind FunctionAST::GetKind() const
	{ return ASTKind::Function; }

	WeakRef<const Token_t> FunctionAST::GetToken() const
	{ return m_Name; }

	GlobalRef<const Token_t> FunctionAST::GetType() const
	{ return m_Type; }

	GlobalRef<const Token_t> FunctionAST::GetName() const
	{ return m_Name; }


	WeakRef<const Token_t> FunctionAST::GetOpen() const
	{ return m_Open; }

	WeakRef<const Token_t> FunctionAST::GetClose() const
	{ return m_Close; }

	GlobalRef<const StatementAST> FunctionAST::GetBody() const
	{ return m_Body; }
	
	void FunctionAST::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Body->Print(os << '\n', indent, "Body = ");
	}
}