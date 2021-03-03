#pragma once

#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	class SYC_API AST;
	class SYC_API CompilationUnitAST;

	class SYC_API MemberAST;
	class SYC_API FunctionAST;

	class SYC_API StatementAST;

	class SYC_API ExpressionAST;

	using MemberList = std::vector<GlobalRef<MemberAST>>;
	using ConstMemberList = std::vector<GlobalRef<const MemberAST>>;

	enum class SYC_API ASTKind
	{
		Unknown,
		CompilationUnit,

		// Members
		Member,
		Function,

		// Statements
		Statement,
		ExpressionStatement,

		// Expressions
		Expression,
		LiteralExpression,

		Count, // Count of ast kinds
	};

	constexpr const char *const ASTKindNames[(uint32)ASTKind::Count] =
	{
		"Unknown",
		"CompilationUnit",


		"Member",
		"Function",


		"Statement",
		"ExpressionStatement",


		"Expression",
		"LiteralExpression",
	};

	class SYC_API AST
	{
	public:
		using Token_t = Token;
	public:
		virtual ASTKind GetKind() const;
		virtual WeakRef<const Token_t> GetToken() const;
		friend std::ostream &operator <<(std::ostream &, const AST &);

		friend std::ostream &operator <<(std::ostream &, const GlobalRef<AST> &);
		friend std::ostream &operator <<(std::ostream &, const WeakRef<AST> &);
		friend std::ostream &operator <<(std::ostream &, const Scope<AST> &);

		SY_PROPERTY_GET(GetKind) ASTKind Kind;
		SY_PROPERTY_GET(GetToken) WeakRef<const Token_t> Token;
	};

	class SYC_API CompilationUnitAST: public AST
	{
	private:
		const MemberList m_Members;
		WeakRef<const Token_t> m_EndOfFile;
	public:
		CompilationUnitAST(const MemberList &members, const WeakRef<const Token_t> &endOfFile);

		virtual ASTKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;


		const MemberList &GetMembers() const;
		WeakRef<const Token_t> GetEndOfFile() const;

		SY_PROPERTY_GET(GetMembers) const MemberList Members;
		SY_PROPERTY_GET(GetEndOfFile) WeakRef<const Token_t> EndOfFile;
	};
}

#include "Symple/Code/AST/MemberAST.h"
#include "Symple/Code/AST/StatementAST.h"
#include "Symple/Code/AST/ExpressionAST.h"