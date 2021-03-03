#pragma once

#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	class SYC_API ExpressionAst: public Ast
	{
	public:
		virtual AstKind GetKind() const override;
	};

	class SYC_API CallExpressionAst: public ExpressionAst
	{
	private:
		GlobalRef<const Token_t> m_Name;
		WeakRef<const Token_t> m_Open;
		ExpressionList m_Params;
		WeakRef<const Token_t> m_Close;
	public:
		CallExpressionAst(GlobalRef<const Token_t> name, WeakRef<const Token_t> open, ExpressionList parameters, WeakRef<const Token_t> close);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const Token_t> GetName() const;
		WeakRef<const Token_t> GetOpen() const;
		ExpressionList GetParameters() const;
		WeakRef<const Token_t> GetClose() const;

		SY_PROPERTY_GET(GetName) GlobalRef<const Token_t> Name;
		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetParameters) ExpressionList Parameters;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;
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
		WeakRef<const Token_t> m_Open;
		GlobalRef<ExpressionAst> m_Expr;
		WeakRef<const Token_t> m_Close;
	public:
		ParenthasizedExpressionAst(WeakRef<const Token_t> open, GlobalRef<ExpressionAst> expression, WeakRef<const Token_t> close);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		WeakRef<const Token_t> GetOpen() const;
		GlobalRef<const ExpressionAst> GetExpression() const;
		WeakRef<const Token_t> GetClose() const;

		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetExpression) GlobalRef<const ExpressionAst> Expression;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;
	};
}