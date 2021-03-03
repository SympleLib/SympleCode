#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API ExpressionAST: public AST
	{
	public:
		virtual ASTKind GetKind() const override;
	};

	class SYC_API LiteralExpressionAST: public ExpressionAST
	{
	private:
		GlobalRef<const Token_t> m_Literal;
	public:
		LiteralExpressionAST(GlobalRef<const Token_t> literal);

		virtual ASTKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;

		GlobalRef<const Token_t> GetLiteral() const;
	};
}