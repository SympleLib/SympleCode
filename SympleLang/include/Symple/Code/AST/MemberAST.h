#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API MemberAST: public AST
	{
	public:
		virtual ASTKind GetKind() const override;
	};

	class SYC_API FunctionAST: public MemberAST
	{
	private:
		GlobalRef<const Token_t> m_Type;
		GlobalRef<const Token_t> m_Name;
		WeakRef<const Token_t> m_Open;
		WeakRef<const Token_t> m_Close;
		GlobalRef<const StatementAST> m_Body;
	public:
		FunctionAST(const GlobalRef<const Token_t> &type, const GlobalRef<const Token_t> &name,
			const WeakRef<const Token_t> &open, const WeakRef<const Token_t> &close, const GlobalRef<const StatementAST> &body);

		virtual ASTKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		
		GlobalRef<const Token_t> GetType() const;
		GlobalRef<const Token_t> GetName() const;
		WeakRef<const Token_t> GetOpen() const;
		WeakRef<const Token_t> GetClose() const;
		GlobalRef<const StatementAST> GetBody() const;

		SY_PROPERTY_GET(GetType) GlobalRef<const Token_t> Type;
		SY_PROPERTY_GET(GetName) GlobalRef<const Token_t> Name;
		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;
		SY_PROPERTY_GET(GetBody) GlobalRef<const StatementAST> Body;
	};
}