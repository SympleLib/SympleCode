#pragma once

#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	class SYC_API MemberAst: public Ast
	{
	public:
		virtual AstKind GetKind() const override;
	};

	class SYC_API Proto: public MemberAst, public Symbol
	{
	protected:
		GlobalRef<const Token_t> m_Name;
		ConstTokenList m_Mods;

		Proto(const GlobalRef<const Token_t> &name, const ConstTokenList &modifiers);
		VISIT_ME;
	public:

		virtual bool GetIsFunction() const override;

		GlobalRef<const Token_t> GetName() const override;
		const ConstTokenList &GetModifiers() const;

		SY_PROPERTY_GET(GetName) GlobalRef<const Token_t> Name;
		SY_PROPERTY_GET(GetModifiers) const ConstTokenList &Modifiers;
	};

	class SYC_API ProtoAst: public Proto
	{
	private:
		GlobalRef<StatementAst> m_Body;

		VISIT_ME;
	public:
		ProtoAst(const GlobalRef<const Token_t> &name, const ConstTokenList &modifiers, const GlobalRef<StatementAst> &body);

		virtual AstKind GetKind() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const StatementAst> GetBody() const;

		SY_PROPERTY_GET(GetBody) GlobalRef<const StatementAst> Body;
	};

	class SYC_API Function: public Proto
	{
	protected:
		GlobalRef<TypeAst> m_Type;
		WeakRef<const Token_t> m_Open;
		ParameterList m_Params;
		WeakRef<const Token_t> m_Close;

		bool m_Main = false;
		TokenKind m_Call = TokenKind::SyCallKeyword;

		Function(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
			const WeakRef<const Token_t> &open, const ParameterList &params, const WeakRef<const Token_t> &close,
				const ConstTokenList &mods);

		VISIT_ME;
	public:
		virtual WeakRef<const Token_t> GetToken() const override;

		TokenKind GetCallingConvention() const;

		GlobalRef<const TypeAst> GetType() const override;
		WeakRef<const Token_t> GetOpen() const;
		const ParameterList &GetParameters() const;
		WeakRef<const Token_t> GetClose() const;

		bool GetIsMain() const;

		SY_PROPERTY_GET(GetCallingConvention) TokenKind CallingConvention;

		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetParameters) const ParameterList &Parameters;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;

		SY_PROPERTY_GET(GetIsMain) bool IsMain;
	};

	class SYC_API FunctionAst: public Function
	{
	private:
		GlobalRef<StatementAst> m_Body;

		VISIT_ME;
	public:
		FunctionAst(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
			const WeakRef<const Token_t> &open, const ParameterList &parameters, const WeakRef<const Token_t> &close,
				const ConstTokenList &modifiers, const GlobalRef<StatementAst> &body);

		virtual AstKind GetKind() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;
		
		GlobalRef<const StatementAst> GetBody() const;

		SY_PROPERTY_GET(GetBody) GlobalRef<const StatementAst> Body;
	};

	class SYC_API ExternFunctionAst: public Function
	{
	private:
		WeakRef<const Token_t> m_Keyword;

		VISIT_ME;
	public:
		ExternFunctionAst(const WeakRef<const Token_t> &keyword, const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
			const WeakRef<const Token_t> &open, const ParameterList &parameters, const WeakRef<const Token_t> &close,
				const ConstTokenList &modifiers);

		virtual AstKind GetKind() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		WeakRef<const Token_t> GetKeyword() const;

		SY_PROPERTY_GET(GetKeyword) WeakRef<const Token_t> Keyword;
	};
}