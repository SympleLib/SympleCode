#pragma once

#include "Symple/Code/Parse/Token.h"
#include "Symple/Code/Util/Printable.h"

namespace Symple::Code
{
	class SYC_API Ast;
	class SYC_API CompilationUnitAst;

	class SYC_API MemberAst;
	class SYC_API FunctionAst;

	class SYC_API StatementAst;
	class SYC_API BlockStatementAst;
	class SYC_API ReturnStatementAst;
	class SYC_API ExpressionStatementAst;

	class SYC_API ExpressionAst;
	class SYC_API BinaryExpressionAst;
	class SYC_API LiteralExpressionAst;
	class SYC_API ParenthasizedExpressionAst;

	using MemberList = std::vector<GlobalRef<MemberAst>>;
	using ConstMemberList = std::vector<GlobalRef<const MemberAst>>;

	using StatementList = std::vector<GlobalRef<StatementAst>>;
	using ConstStatementList = std::vector<GlobalRef<const StatementAst>>;

	enum class SYC_API AstKind
	{
		Unknown,
		CompilationUnit,

		// Members
		Member,
		Function,

		// Statements
		Statement,
		BlockStatement,
		ReturnStatement,
		ExpressionStatement,

		// Expressions
		Expression,
		UnaryExpression,
		BinaryExpression,
		LiteralExpression,
		ParenthasizedExpression,

		Count, // Count of ast kinds
	};

	constexpr const char *const AstKindNames[(uint32)AstKind::Count] =
	{
		"Unknown",
		"CompilationUnit",


		"Member",
		"Function",


		"Statement",
		"BlockStatement",
		"ReturnStatement",
		"ExpressionStatement",


		"Expression",
		"UnaryExpression",
		"BinaryExpression",
		"LiteralExpression",
		"ParenthasizedExpression",
	};

	SYC_API std::ostream &operator <<(std::ostream &, AstKind);

	class SYC_API Ast: public Printable
	{
	protected:
		void PrintKind(std::ostream &) const;
	public:
		using Token_t = Token;
	public:
		virtual AstKind GetKind() const;
		virtual WeakRef<const Token_t> GetToken() const;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		SY_PROPERTY_GET(GetKind) AstKind Kind;
		SY_PROPERTY_GET(GetToken) WeakRef<const Token_t> Token;
	};

	class SYC_API CompilationUnitAst: public Ast
	{
	private:
		MemberList m_Members;
		WeakRef<const Token_t> m_EndOfFile;
	public:
		CompilationUnitAst(const MemberList &members, const WeakRef<const Token_t> &endOfFile);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		const MemberList &GetMembers() const;
		WeakRef<const Token_t> GetEndOfFile() const;

		SY_PROPERTY_GET(GetMembers) const MemberList Members;
		SY_PROPERTY_GET(GetEndOfFile) WeakRef<const Token_t> EndOfFile;
	};
}

#include "Symple/Code/Ast/MemberAst.h"
#include "Symple/Code/Ast/StatementAst.h"
#include "Symple/Code/Ast/ExpressionAst.h"