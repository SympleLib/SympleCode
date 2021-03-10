#pragma once

#include "Symple/Code/Ast/Ast.h"

namespace Symple::Code
{
	class SYC_API ExpressionAst: public Ast
	{
	public:
		using Type_t = Type;
	protected:
		GlobalRef<const Type_t> m_Type = Type_t::Default;
		friend class TypeVisitor;
	public:
		virtual AstKind GetKind() const override;

		GlobalRef<const Type_t> GetType() const;

		SY_PROPERTY_GET(GetType) GlobalRef<const Type_t> Type;
	};

	class SYC_API CallExpressionAst: public ExpressionAst
	{
	private:
		GlobalRef<const Token_t> m_Name;
		WeakRef<const Token_t> m_Open;
		ExpressionList m_Params;
		WeakRef<const Token_t> m_Close;

		GlobalRef<FunctionAst> m_Func;

		friend class SymbolVisitor;
		friend class TypeVisitor;
	public:
		CallExpressionAst(GlobalRef<const Token_t> name, WeakRef<const Token_t> open, const ExpressionList &parameters, WeakRef<const Token_t> close);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const Token_t> GetName() const;
		WeakRef<const Token_t> GetOpen() const;
		const ExpressionList &GetParameters() const;
		WeakRef<const Token_t> GetClose() const;

		GlobalRef<const FunctionAst> GetFunction() const;

		SY_PROPERTY_GET(GetName) GlobalRef<const Token_t> Name;
		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetParameters) const ExpressionList &Parameters;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;

		SY_PROPERTY_GET(GetFunction) GlobalRef<const FunctionAst> Function;
	};

	class SYC_API CastExpressionAst: public ExpressionAst
	{
	public:
		using TypeAst_t = TypeAst;
	private:
		WeakRef<const Token_t> m_Open;
		GlobalRef<TypeAst> m_TypeAst;
		WeakRef<const Token_t> m_Close;
		GlobalRef<ExpressionAst> m_Value;

		friend class SymbolVisitor;
		friend class TypeVisitor;
	public:
		CastExpressionAst(WeakRef<const Token_t> open, GlobalRef<TypeAst_t> type, WeakRef<const Token_t> close, GlobalRef<ExpressionAst> value);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		WeakRef<const Token_t> GetOpen() const;
		GlobalRef<const TypeAst_t> GetTypeAst() const;
		WeakRef<const Token_t> GetClose() const;
		GlobalRef<const ExpressionAst> GetValue() const;

		SY_PROPERTY_GET(GetOpen) WeakRef<const Token_t> Open;
		SY_PROPERTY_GET(GetTypeAst) GlobalRef<const TypeAst_t> TypeAst;
		SY_PROPERTY_GET(GetClose) WeakRef<const Token_t> Close;
		SY_PROPERTY_GET(GetValue) GlobalRef<const ExpressionAst> Value;
	};

	class SYC_API UnaryExpressionAst: public ExpressionAst
	{
	private:
		GlobalRef<const Token_t> m_Operator;
		GlobalRef<ExpressionAst> m_Operand;

		friend class SymbolVisitor;
		friend class TypeVisitor;
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
	
		friend class SymbolVisitor;
		friend class TypeVisitor;
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

		friend class TypeVisitor;
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

		friend class TypeVisitor;
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