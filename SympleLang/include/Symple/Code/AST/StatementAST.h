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
		friend class TypeVisitor;
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

	class SYC_API VariableStatementAst: public StatementAst
	{
	private:
		GlobalRef<TypeAst> m_Type;
		GlobalRef<const Token_t> m_Name;
		WeakRef<const Token_t> m_Equals;
		GlobalRef<ExpressionAst> m_Init;

		friend class SymbolVisitor;
		friend class TypeVisitor;
	public:
		VariableStatementAst(GlobalRef<TypeAst> type, GlobalRef<const Token_t> name, WeakRef<const Token_t> equals = {}, GlobalRef<ExpressionAst> initializer = ExpressionAst::DefaultExpression);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const TypeAst> GetType() const;
		GlobalRef<const Token_t> GetName() const;
		WeakRef<const Token_t> GetEquals() const;
		GlobalRef<const ExpressionAst> GetInitializer() const;

		SY_PROPERTY_GET(GetType) GlobalRef<const TypeAst> Type;
		SY_PROPERTY_GET(GetName) GlobalRef<const Token_t> Name;
		SY_PROPERTY_GET(GetEquals) WeakRef<const TypeAst> Equals;
		SY_PROPERTY_GET(GetInitializer) GlobalRef<const ExpressionAst> Initializer;
	};

	class SYC_API ExpressionStatementAst: public StatementAst
	{
	private:
		GlobalRef<ExpressionAst> m_Expr;

		friend class SymbolVisitor;
		friend class TypeVisitor;
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
		friend class TypeVisitor;
	public:
		ReturnStatementAst(WeakRef<const Token_t> keyword, GlobalRef<ExpressionAst> value);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const ExpressionAst> GetValue() const;

		SY_PROPERTY_GET(GetValue) GlobalRef<const ExpressionAst> Value;
	};
}