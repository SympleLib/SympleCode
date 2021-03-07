#include "Symple/Code/Ast/ExpressionAst.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;
	using Type_t = ExpressionAst::Type_t;

	AstKind ExpressionAst::GetKind() const
	{ return AstKind::Expression; }

	GlobalRef<const Type_t> ExpressionAst::GetType() const
	{ return m_Type; }


	CallExpressionAst::CallExpressionAst(GlobalRef<const Token_t> name, WeakRef<const Token_t> open, const ExpressionList &params, WeakRef<const Token_t> close)
		: m_Name(name), m_Open(open), m_Params(params), m_Close(close) {}

	AstKind CallExpressionAst::GetKind() const
	{ return AstKind::CallExpression; }

	WeakRef<const Token_t> CallExpressionAst::GetToken() const
	{ return m_Open; }

	GlobalRef<const Token_t> CallExpressionAst::GetName() const
	{ return m_Name; }

	WeakRef<const Token_t> CallExpressionAst::GetOpen() const
	{ return m_Open; }

	const ExpressionList &CallExpressionAst::GetParameters() const
	{ return m_Params; }

	WeakRef<const Token_t> CallExpressionAst::GetClose() const
	{ return m_Close; }


	GlobalRef<const FunctionAst> CallExpressionAst::GetFunction() const
	{ return m_Func; }

	void CallExpressionAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Name->Print(os << '\n', indent, "Name = ", false);
		if (!m_Open.expired())
			m_Open.lock()->Print(os << '\n', indent, "Open = ", false);
		for (auto param : m_Params)
			param->Print(os << '\n', indent, "[Param] ", m_Close.expired());
		if (!m_Close.expired())
			m_Close.lock()->Print(os << '\n', indent, "Close = ");
	}


	UnaryExpressionAst::UnaryExpressionAst(GlobalRef<const Token_t> op, GlobalRef<ExpressionAst> operand)
		: m_Operator(op), m_Operand(operand) {}

	AstKind UnaryExpressionAst::GetKind() const
	{ return AstKind::UnaryExpression; }

	WeakRef<const Token_t> UnaryExpressionAst::GetToken() const
	{ return m_Operator; }

	GlobalRef<const Token_t> UnaryExpressionAst::GetOperator() const
	{ return m_Operator; }

	GlobalRef<const ExpressionAst> UnaryExpressionAst::GetOperand() const
	{ return m_Operand; }

	void UnaryExpressionAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Operator->Print(os << '\n', indent, "Operator = ", false);
		m_Operand->Print(os << '\n', indent, "Operand = ");
	}


	BinaryExpressionAst::BinaryExpressionAst(GlobalRef<const Token_t> op, GlobalRef<ExpressionAst> left, GlobalRef<ExpressionAst> right)
		: m_Operator(op), m_Left(left), m_Right(right) {}

	AstKind BinaryExpressionAst::GetKind() const
	{ return AstKind::BinaryExpression; }

	WeakRef<const Token_t> BinaryExpressionAst::GetToken() const
	{ return m_Operator; }

	GlobalRef<const Token_t> BinaryExpressionAst::GetOperator() const
	{ return m_Operator; }

	GlobalRef<const ExpressionAst> BinaryExpressionAst::GetLeft() const
	{ return m_Left; }

	GlobalRef<const ExpressionAst> BinaryExpressionAst::GetRight() const
	{ return m_Right; }

	void BinaryExpressionAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Operator->Print(os << '\n', indent, "Operator = ", false);
		m_Left->Print(os << '\n', indent, "Left = ", false);
		m_Right->Print(os << '\n', indent, "Right = ");
	}


	LiteralExpressionAst::LiteralExpressionAst(GlobalRef<const Token_t> literal)
		: m_Literal(literal) {}

	AstKind LiteralExpressionAst::GetKind() const
	{ return AstKind::LiteralExpression; }

	WeakRef<const Token_t> LiteralExpressionAst::GetToken() const
	{ return m_Literal; }

	GlobalRef<const Token_t> LiteralExpressionAst::GetLiteral() const
	{ return m_Literal; }

	void LiteralExpressionAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Literal->Print(os << '\n', indent, "Literal = ");
	}


	ParenthasizedExpressionAst::ParenthasizedExpressionAst(WeakRef<const Token_t> open, GlobalRef<ExpressionAst> expr, WeakRef<const Token_t> close)
		: m_Open(open), m_Expr(expr), m_Close(close) {}

	AstKind ParenthasizedExpressionAst::GetKind() const
	{ return AstKind::ParenthasizedExpression; }

	WeakRef<const Token_t> ParenthasizedExpressionAst::GetToken() const
	{ return m_Open; }

	WeakRef<const Token_t> ParenthasizedExpressionAst::GetOpen() const
	{ return m_Open; }

	GlobalRef<const ExpressionAst> ParenthasizedExpressionAst::GetExpression() const
	{ return m_Expr; }

	WeakRef<const Token_t> ParenthasizedExpressionAst::GetClose() const
	{ return m_Close; }

	void ParenthasizedExpressionAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		if (!m_Open.expired())
			m_Open.lock()->Print(os << '\n', indent, "Open = ", false);
		m_Expr->Print(os << '\n', indent, "Expression = ", false);
		if (!m_Close.expired())
			m_Close.lock()->Print(os << '\n', indent, "Close = ");
	}
}