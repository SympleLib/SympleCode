#pragma once

#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	class SYC_API StatementAst: public Ast
	{
	public:
		virtual AstKind GetKind() const override;
	};

	class SYC_API ExpressionStatementAst: public StatementAst
	{
	private:
		GlobalRef<ExpressionAst> m_Expr;
	public:
		ExpressionStatementAst(GlobalRef<ExpressionAst> expression);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const ExpressionAst> GetExpression() const;

		SY_PROPERTY_GET(GetExpression) GlobalRef<const ExpressionAst> Expression;
	};

	class SYC_API ReturnStatementAst: public StatementAst
	{
	private:
		WeakRef<const Token_t> m_Keyword;
		GlobalRef<ExpressionAst> m_Value;
	public:
		ReturnStatementAst(WeakRef<const Token_t> keyword, GlobalRef<ExpressionAst> value);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const ExpressionAst> GetValue() const;

		SY_PROPERTY_GET(GetValue) GlobalRef<const ExpressionAst> Value;
	};
}