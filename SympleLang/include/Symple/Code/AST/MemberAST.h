#pragma once

#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	class SYC_API MemberAst: public Ast
	{
	public:
		virtual AstKind GetKind() const override;
	};

	class SYC_API FunctionAst: public MemberAst, public Symbol
	{
	private:
		GlobalRef<TypeAst> m_Type;
		GlobalRef<const Token_t> m_Name;
		WeakRef<const Token_t> m_Open;
		ParameterList m_Params;
		WeakRef<const Token_t> m_Close;
		ConstTokenList m_Mods;
		GlobalRef<StatementAst> m_Body;

		VISIT_ME;
	public:
		FunctionAst(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
			const WeakRef<const Token_t> &open, const ParameterList &parameters, const WeakRef<const Token_t> &close,
				const ConstTokenList &modifiers, const GlobalRef<StatementAst> &body);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;
		virtual bool GetIsFunction() const override;
		
		GlobalRef<const TypeAst> GetType() const override;
		GlobalRef<const Token_t> GetName() const override;
		WeakRef<const Token_t> GetOpen() const;
		const ParameterList &GetParameters() const;
		WeakRef<const Token_t> GetClose() const;
		const ConstTokenList &GetModifiers() const;
		GlobalRef<const StatementAst> GetBody() const;

		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetParameters) const ParameterList &Parameters;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;
		SY_PROPERTY_GET(GetBody) GlobalRef<const StatementAst> Body;
		SY_PROPERTY_GET(GetModifiers) const ConstTokenList &Modifiers;
	};

	class SYC_API ExternFunctionAst: public MemberAst, public Symbol
	{
	private:
		WeakRef<const Token_t> m_Keyword;
		GlobalRef<TypeAst> m_Type;
		GlobalRef<const Token_t> m_Name;
		WeakRef<const Token_t> m_Open;
		ParameterList m_Params;
		WeakRef<const Token_t> m_Close;
		ConstTokenList m_Mods;

		VISIT_ME;
	public:
		ExternFunctionAst(const WeakRef<const Token_t> &keyword, const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
			const WeakRef<const Token_t> &open, const ParameterList &parameters, const WeakRef<const Token_t> &close,
				const ConstTokenList &modifiers);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;
		virtual bool GetIsFunction() const override;

		WeakRef<const Token_t> GetKeyword() const;
		GlobalRef<const TypeAst> GetType() const override;
		GlobalRef<const Token_t> GetName() const override;
		WeakRef<const Token_t> GetOpen() const;
		const ParameterList &GetParameters() const;
		WeakRef<const Token_t> GetClose() const;
		const ConstTokenList &GetModifiers() const;

		SY_PROPERTY_GET(GetKeyword) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetParameters) const ParameterList &Parameters;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;
		SY_PROPERTY_GET(GetModifiers) const ConstTokenList &Modifiers;
	};
}