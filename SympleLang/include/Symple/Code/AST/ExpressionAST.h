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
}