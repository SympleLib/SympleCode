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
		GlobalRef<StatementAst> m_Body;

		friend class SymbolVisitor;
		friend class TypeVisitor;
	public:
		FunctionAst(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name,
			const WeakRef<const Token_t> &open, const ParameterList &parameters, const WeakRef<const Token_t> &close, const GlobalRef<StatementAst> &body);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;
		virtual bool GetIsFunction() const override;
		
		GlobalRef<const TypeAst> GetType() const override;
		GlobalRef<const Token_t> GetName() const override;
		WeakRef<const Token_t> GetOpen() const;
		const ParameterList &GetParameters() const;
		WeakRef<const Token_t> GetClose() const;
		GlobalRef<const StatementAst> GetBody() const;

		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetParameters) const ParameterList &Parameters;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;
		SY_PROPERTY_GET(GetBody) GlobalRef<const StatementAst> Body;
	};
}