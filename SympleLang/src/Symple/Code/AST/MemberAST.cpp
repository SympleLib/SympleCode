#include "Symple/Code/Ast/MemberAst.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;

	AstKind MemberAst::GetKind() const
	{ return AstKind::Member; }


	Function::Function(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
		const WeakRef<const Token_t> &open, const ParameterList &params, const WeakRef<const Token_t> &close,
			const ConstTokenList &mods)
		: m_Type(type), m_Name(name), m_Open(open), m_Params(params), m_Close(close), m_Mods(mods)
	{}

	WeakRef<const Token_t> Function::GetToken() const
	{ return m_Name; }

	GlobalRef<const TypeAst> Function::GetType() const
	{ return m_Type; }

	GlobalRef<const Token_t> Function::GetName() const
	{ return m_Name; }

	bool Function::GetIsFunction() const
	{ return true; }


	WeakRef<const Token_t> Function::GetOpen() const
	{ return m_Open; }

	const ParameterList &Function::GetParameters() const
	{ return m_Params; }

	WeakRef<const Token_t> Function::GetClose() const
	{ return m_Close; }

	const ConstTokenList &Function::GetModifiers() const
	{ return m_Mods; }


	FunctionAst::FunctionAst(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
		const WeakRef<const Token_t> &open, const ParameterList &params, const WeakRef<const Token_t> &close,
			const ConstTokenList &mods, const GlobalRef<StatementAst> &body)
		: Function(type, name, open, params, close, mods), m_Body(body)
	{}

	AstKind FunctionAst::GetKind() const
	{ return AstKind::Function; }

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
		for (auto mod : m_Mods)
			mod->Print(os << '\n', indent, "[Modifiers] ", false);
		m_Body->Print(os << '\n', indent, "Body = ");
	}


	ExternFunctionAst::ExternFunctionAst(const WeakRef<const Token_t> &keyword, const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
		const WeakRef<const Token_t> &open, const ParameterList &params, const WeakRef<const Token_t> &close,
			const ConstTokenList &mods)
		: m_Keyword(keyword), Function(type, name, open, params, close, mods)
	{}

	AstKind ExternFunctionAst::GetKind() const
	{ return AstKind::ExternFunction; }

	void ExternFunctionAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);
		os << " (" << m_MangledName << ')';

		indent += GetAddIndent(last);
		if (!m_Keyword.expired())
			m_Keyword.lock()->Print(os << '\n', indent, "Keyword = ", false);
		m_Type->Print(os << '\n', indent, "Type = ", false);
		m_Name->Print(os << '\n', indent, "Name = ", false);
		if (!m_Open.expired())
			m_Open.lock()->Print(os << '\n', indent, "Open = ", false);
		for (auto param : m_Params)
			param->Print(os << '\n', indent, "[Param] ", false);
		if (!m_Close.expired())
			m_Close.lock()->Print(os << '\n', indent, "Close = ", false);
		for (auto mod : m_Mods)
			mod->Print(os << '\n', indent, "[Modifiers] ", mod == m_Mods.back());
	}
}