#include "Symple/Code/Ast/MemberAst.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;

	AstKind MemberAst::GetKind() const
	{ return AstKind::Member; }


	Proto::Proto(const GlobalRef<const Token_t> &name, const ConstTokenList &mods)
		: m_Name(name), m_Mods(mods) {}

	bool Proto::GetIsFunction() const
	{ return true; }

	GlobalRef<const Token_t> Proto::GetName() const
	{ return m_Name; }

	const ConstTokenList &Proto::GetModifiers() const
	{ return m_Mods; }


	StructAst::StructAst(const WeakRef<const Token_t> &keyword, const GlobalRef<const Token_t> &name,
		const WeakRef<const Token_t> &open, GlobalRef<VariableStatementAst> fields, const WeakRef<const Token_t> &close)
		: TypeBase(name->Text), m_Kerword(keyword), m_NameTok(name), m_Open(open), m_Fields(fields), m_Close(close),
			m_Sz()
	{
		for (auto field : fields->Declarations)
			m_Sz += field->Type->Type->Size;
	}

	uint32 StructAst::GetSize() const
	{ return m_Sz; }

	AstKind StructAst::GetKind() const
	{ return AstKind::Struct; }

	WeakRef<const Token_t> StructAst::GetKeyword() const
	{ return m_Kerword; }

	WeakRef<const Token_t> StructAst::GetNameToken() const
	{ return m_NameTok; }

	WeakRef<const Token_t> StructAst::GetOpen() const
	{ return m_Open; }

	GlobalRef<const VariableStatementAst> StructAst::GetFields() const
	{ return m_Fields; }

	WeakRef<const Token_t> StructAst::GetClose() const
	{ return m_Close; }

	void StructAst::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);
		os << " (" << m_Name << ')';

		indent += GetAddIndent(last); //                  / This is not a typo (intentional)
		if (!m_Kerword.expired()) //                     \|/ 
			m_Kerword.lock()->Print(os << '\n', indent, "Kerword = ", false);
		if (!m_NameTok.expired())
			m_NameTok.lock()->Print(os << '\n', indent, "Name = ", false);
		if (!m_Open.expired())
			m_Open.lock()->Print(os << '\n', indent, "Open = ", false);
		m_Fields->Print(os << '\n', indent, "Fields = ", m_Close.expired());
		if (!m_Close.expired())
			m_Close.lock()->Print(os << '\n', indent, "Close = ");
	}


	Function::Function(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
		const WeakRef<const Token_t> &open, const ParameterList &params, const WeakRef<const Token_t> &close,
		const ConstTokenList &mods)
		: Proto(name, mods), m_Type(type), m_Open(open), m_Params(params), m_Close(close)
	{}

	WeakRef<const Token_t> Function::GetToken() const
	{ return m_Name; }

	GlobalRef<const TypeAst> Function::GetType() const
	{ return m_Type; }


	TokenKind Function::GetCallingConvention() const
	{ return m_Call; }


	WeakRef<const Token_t> Function::GetOpen() const
	{ return m_Open; }

	const ParameterList &Function::GetParameters() const
	{ return m_Params; }

	WeakRef<const Token_t> Function::GetClose() const
	{ return m_Close; }

	bool Function::GetIsMain() const
	{ return m_Main; }


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
			param->Print(os << '\n', indent, "[Parameter] ", false);
		if (!m_Close.expired())
			m_Close.lock()->Print(os << '\n', indent, "Close = ", false);
		for (auto mod : m_Mods)
			mod->Print(os << '\n', indent, "[Modifier] ", false);
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