#pragma once

#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	class SYC_API StatementAst: public Ast
	{
	public:
		virtual AstKind GetKind() const override;
	};

	class SYC_API EmptyStatementAst: public StatementAst
	{
	private:
		WeakRef<const Token_t> m_Semi;
	public:
		EmptyStatementAst(WeakRef<const Token_t> semicolon);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		WeakRef<const Token_t> GetSemicolon() const;

		SY_PROPERTY_GET(GetSemicolon) WeakRef<const Token_t> Semicolon;
	};

	class SYC_API BlockStatementAst: public StatementAst
	{
	private:
		WeakRef<const Token_t> m_Open;
		StatementList m_Stmts;
		WeakRef<const Token_t> m_Close;

		friend class SymbolVisitor;
	public:
		BlockStatementAst(WeakRef<const Token_t> open, StatementList statements, WeakRef<const Token_t> close);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		WeakRef<const Token_t> GetOpen() const;
		StatementList GetStatements() const;
		WeakRef<const Token_t> GetClose() const;

		SY_PROPERTY_GET(GetStatements) StatementList Statements;
	};

	class SYC_API ExpressionStatementAst: public StatementAst
	{
	private:
		GlobalRef<ExpressionAst> m_Expr;

		friend class SymbolVisitor;
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

		friend class SymbolVisitor;
	public:
		ReturnStatementAst(WeakRef<const Token_t> keyword, GlobalRef<ExpressionAst> value);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const ExpressionAst> GetValue() const;

		SY_PROPERTY_GET(GetValue) GlobalRef<const ExpressionAst> Value;
	};
}