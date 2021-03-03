#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API StatementAST: public AST
	{
	public:
		virtual ASTKind GetKind() const override;
	};

	class SYC_API ExpressionStatementAST: public AST
	{
	private:
		GlobalRef<ExpressionAST> m_Expr;
	public:
		ExpressionStatementAST(GlobalRef<ExpressionAST> expression);

		virtual ASTKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;

		GlobalRef<const ExpressionAST> GetExpression() const;

		SY_PROPERTY_GET(GetExpression) GlobalRef<const ExpressionAST> Expression;
	};
}