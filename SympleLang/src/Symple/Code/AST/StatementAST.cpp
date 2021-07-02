#include "Symple/Code/Ast/StatementAst.h"

namespace Symple::Code
{
	using Token_t = Ast::Token_t;

	AstKind StatementAst::GetKind() const
	{ return AstKind::Statement; }


	EmptyStatementAst::EmptyStatementAst(WeakRef<const Token_t> semi)
		: m_Semi(semi) {}

	AstKind EmptyStatementAst::GetKind() const
	{ return AstKind::EmptyStatement; }

	WeakRef<const Token_t> EmptyStatementAst::GetToken() const
	{ return m_Semi; }

	WeakRef<const Token_t> EmptyStatementAst::GetSemicolon() const
	{ return m_Semi; }

	void EmptyStatementAst::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		if (!m_Semi.expired())
			m_Semi.lock()->Print(os << '\n', indent, "Semicolon = ");
	}


	BlockStatementAst::BlockStatementAst(WeakRef<const Token_t> open, StatementList stmts, WeakRef<const Token_t> close)
		: m_Open(open), m_Stmts(stmts), m_Close(close) {}

	AstKind BlockStatementAst::GetKind() const
	{ return AstKind::BlockStatement; }

	WeakRef<const Token_t> BlockStatementAst::GetToken() const
	{ return m_Open; }

	WeakRef<const Token_t> BlockStatementAst::GetOpen() const
	{ return m_Open; }

	StatementList BlockStatementAst::GetStatements() const
	{ return m_Stmts; }

	WeakRef<const Token_t> BlockStatementAst::GetClose() const
	{ return m_Close; }

	void BlockStatementAst::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		if (!m_Open.expired())
			m_Open.lock()->Print(os << '\n', indent, "Open = ", false);
		for (auto stmt : m_Stmts)
			stmt->Print(os << '\n', indent, "[Statement] ", false);
		if (!m_Close.expired())
			m_Close.lock()->Print(os << '\n', indent, "Close = ");
	}


	VariableStatementAst::VariableStatementAst(GlobalRef<TypeAst> ty, GlobalRef<const Token_t> name, WeakRef<const Token_t> equals, GlobalRef<ExpressionAst> init, GlobalRef<VariableStatementAst> next)
		: m_Type(ty), m_Name(name), m_Equals(equals), m_Init(init), m_Next(next) {}

	AstKind VariableStatementAst::GetKind() const
	{ return AstKind::VariableStatement; }

	WeakRef<const Token_t> VariableStatementAst::GetToken() const
	{ return m_Name; }

	GlobalRef<const TypeAst> VariableStatementAst::GetType() const
	{ return m_Type; }

	GlobalRef<const Token_t> VariableStatementAst::GetName() const
	{ return m_Name; }

	WeakRef<const Token_t> VariableStatementAst::GetEquals() const
	{ return m_Equals; }

	GlobalRef<const ExpressionAst> VariableStatementAst::GetInitializer() const
	{ return m_Init; }

	GlobalRef<const VariableStatementAst> VariableStatementAst::GetNext() const
	{ return m_Next; }

	void VariableStatementAst::SetNext(GlobalRef<VariableStatementAst> to)
	{ m_Next = to; }

	uint32 VariableStatementAst::GetDepth() const
	{ return m_Depth; }

	void VariableStatementAst::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);
		os << " [Depth " << m_Depth << ']';

		indent += GetAddIndent(last);
		m_Type->Print(os << '\n', indent, "Type = ", false);
		m_Name->Print(os << '\n', indent, "Name = ", m_Equals.expired() && !m_Init);
		if (!m_Equals.expired())
			m_Equals.lock()->Print(os << '\n', indent, "Equals = ", !m_Init);
		if (m_Init)
			m_Init->Print(os << '\n', indent, "Initializer = ", !m_Next);
		if (m_Next)
			m_Next->Print(os << '\n', indent, "Next = ");
	}


	ExpressionStatementAst::ExpressionStatementAst(GlobalRef<ExpressionAst> expr)
		: m_Expr(expr) {}

	AstKind ExpressionStatementAst::GetKind() const
	{ return AstKind::ExpressionStatement; }

	WeakRef<const Token_t> ExpressionStatementAst::GetToken() const
	{ return m_Expr->Token; }

	GlobalRef<const ExpressionAst> ExpressionStatementAst::GetExpression() const
	{ return m_Expr; }

	void ExpressionStatementAst::Print(std::ostream & os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Expr->Print(os << '\n', indent, "Expression = ");
	}


	ReturnStatementAst::ReturnStatementAst(WeakRef<const Token_t> keyword, GlobalRef<ExpressionAst> val)
		: m_Keyword(keyword), m_Value(val) {}

	AstKind ReturnStatementAst::GetKind() const
	{ return AstKind::ReturnStatement; }

	WeakRef<const Token_t> ReturnStatementAst::GetToken() const
	{ return m_Keyword; }

	GlobalRef<const ExpressionAst> ReturnStatementAst::GetValue() const
	{ return m_Value; }

	void ReturnStatementAst::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		PrintKind(os);

		indent += GetAddIndent(last);
		m_Value->Print(os << '\n', indent, "Value = ");
	}
}