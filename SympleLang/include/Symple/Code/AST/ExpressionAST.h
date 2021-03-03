#pragma once

#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	class SYC_API ExpressionAst: public Ast
	{
	public:
		virtual AstKind GetKind() const override;
	};

	class SYC_API UnaryExpressionAst: public ExpressionAst
	{
	private:
		GlobalRef<const Token_t> m_Operator;
		GlobalRef<ExpressionAst> m_Operand;
	public:
		UnaryExpressionAst(GlobalRef<const Token_t> oqerator, GlobalRef<ExpressionAst> operand);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const Token_t> GetOperator() const;
		GlobalRef<const ExpressionAst> GetOperand() const;

		SY_PROPERTY_GET(GetOperator) GlobalRef<const Token_t> Operator;
		SY_PROPERTY_GET(GetOperand) GlobalRef<const ExpressionAst> Operand;
	};

	class SYC_API BinaryExpressionAst: public ExpressionAst
	{
	private:
		GlobalRef<const Token_t> m_Operator;
		GlobalRef<ExpressionAst> m_Left, m_Right;
	public:
		BinaryExpressionAst(GlobalRef<const Token_t> oqerator, GlobalRef<ExpressionAst> left, GlobalRef<ExpressionAst> right);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const Token_t> GetOperator() const;
		GlobalRef<const ExpressionAst> GetLeft() const;
		GlobalRef<const ExpressionAst> GetRight() const;

		SY_PROPERTY_GET(GetOperator) GlobalRef<const Token_t> Operator;
		SY_PROPERTY_GET(GetLeft) GlobalRef<const ExpressionAst> Left;
		SY_PROPERTY_GET(GetRight) GlobalRef<const ExpressionAst> Right;
	};

	class SYC_API LiteralExpressionAst: public ExpressionAst
	{
	private:
		GlobalRef<const Token_t> m_Literal;
	public:
		LiteralExpressionAst(GlobalRef<const Token_t> literal);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const Token_t> GetLiteral() const;

		SY_PROPERTY_GET(GetLiteral) GlobalRef<const Token_t> Literal;
	};

	class SYC_API ParenthasizedExpressionAst: public ExpressionAst
	{
	private:
		GlobalRef<const Token_t> m_Open;
		GlobalRef<ExpressionAst> m_Expr;
		GlobalRef<const Token_t> m_Close;
	public:
		ParenthasizedExpressionAst(GlobalRef<const Token_t> open, GlobalRef<ExpressionAst> expression, GlobalRef<const Token_t> close);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const Token_t> GetOpen() const;
		GlobalRef<const ExpressionAst> GetExpression() const;
		GlobalRef<const Token_t> GetClose() const;

		SY_PROPERTY_GET(GetOpen) GlobalRef<const Token_t> Open;
		SY_PROPERTY_GET(GetExpression) GlobalRef<const ExpressionAst> Expression;
		SY_PROPERTY_GET(GetClose) GlobalRef<const Token_t> Close;
	};
}