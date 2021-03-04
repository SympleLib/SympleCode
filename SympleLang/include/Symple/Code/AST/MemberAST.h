#pragma once

#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	class SYC_API MemberAst: public Ast
	{
	public:
		virtual AstKind GetKind() const override;
	};

	class SYC_API FunctionAst: public MemberAst
	{
	private:
		GlobalRef<const Token_t> m_Type;
		GlobalRef<const Token_t> m_Name;
		WeakRef<const Token_t> m_Open;
		WeakRef<const Token_t> m_Close;
		GlobalRef<StatementAst> m_Body;

		friend class SymbolVisitor;
	public:
		FunctionAst(const GlobalRef<const Token_t> &type, const GlobalRef<const Token_t> &name,
			const WeakRef<const Token_t> &open, const WeakRef<const Token_t> &close, const GlobalRef<StatementAst> &body);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;
		
		GlobalRef<const Token_t> GetType() const;
		GlobalRef<const Token_t> GetName() const;
		WeakRef<const Token_t> GetOpen() const;
		WeakRef<const Token_t> GetClose() const;
		GlobalRef<const StatementAst> GetBody() const;

		SY_PROPERTY_GET(GetType) GlobalRef<const Token_t> Type;
		SY_PROPERTY_GET(GetName) GlobalRef<const Token_t> Name;
		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;
		SY_PROPERTY_GET(GetBody) GlobalRef<const StatementAst> Body;
	};
}