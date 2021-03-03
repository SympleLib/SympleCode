#pragma once

#include "Symple/Code/AST/AST.h"

namespace Symple::Code
{
	class SYC_API StatementAST: public AST
	{
	public:
		virtual ASTKind GetKind() const override;
	};

	class SYC_API ExpressionStatementAST: public StatementAST
	{
	private:
		GlobalRef<ExpressionAST> m_Expr;
	public:
		ExpressionStatementAST(GlobalRef<ExpressionAST> expression);

		virtual ASTKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const ExpressionAST> GetExpression() const;

		SY_PROPERTY_GET(GetExpression) GlobalRef<const ExpressionAST> Expression;
	};

	class SYC_API ReturnStatementAST: public StatementAST
	{
	private:
		WeakRef<const Token_t> m_Keyword;
		GlobalRef<ExpressionAST> m_Value;
	public:
		ReturnStatementAST(WeakRef<const Token_t> keyword, GlobalRef<ExpressionAST> value);

		virtual ASTKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const ExpressionAST> GetValue() const;

		SY_PROPERTY_GET(GetValue) GlobalRef<const ExpressionAST> Value;
	};
}