#include "Symple/Code/Ast/MemberAst.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;

	AstKind MemberAst::GetKind() const
	{ return AstKind::Member; }


	FunctionAst::FunctionAst(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
		const WeakRef<const Token_t> &open, const ParameterList &params, const WeakRef<const Token_t> &close, const GlobalRef<StatementAst> &body)
		: m_Type(type), m_Name(name), m_Params(params), m_Open(open), m_Close(close), m_Body(body) {}

	AstKind FunctionAst::GetKind() const
	{ return AstKind::Function; }

	WeakRef<const Token_t> FunctionAst::GetToken() const
	{ return m_Name; }

	GlobalRef<const TypeAst> FunctionAst::GetType() const
	{ return m_Type; }

	GlobalRef<const Token_t> FunctionAst::GetName() const
	{ return m_Name; }

	bool FunctionAst::GetIsFunction() const
	{ return true; }


	WeakRef<const Token_t> FunctionAst::GetOpen() const
	{ return m_Open; }

	const ParameterList &FunctionAst::GetParameters() const
	{ return m_Params; }

	WeakRef<const Token_t> FunctionAst::GetClose() const
	{ return m_Close; }

	GlobalRef<const StatementAst> FunctionAst::GetBody() const
	{ return m_Body; }
	
	void FunctionAst::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);
		os << " (" << m_MangledName << ')';

		indent += GetAddIndent(last);
		m_Type->Print(os << '\n', indent, "Type = ", false);
		m_Name->Print(os << '\n', indent, "Name = ", false);
		if (!m_Open.expired())
			m_Open.lock()->Print(os << '\n', indent, "Open = ", false);
		for (auto param : m_Params)
			param->Print(os << '\n', indent, "[Param] ", false);
		if (!m_Close.expired())
			m_Close.lock()->Print(os << '\n', indent, "Close = ", false);
		m_Body->Print(os << '\n', indent, "Body = ");
	}
}