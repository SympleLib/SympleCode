#pragma once

#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	enum class SYC_API ASTKind
	{
		Unknown,

		// Members
		Member,
		Function,

		Count, // Count of ast kinds
	};

	constexpr const char *const ASTKindNames[(uint32)ASTKind::Count] =
	{
		"Unknown",


		"Member",
		"Function",
	};

	class SYC_API AST
	{
	public:
		using Token_t = Token;
	public:
		virtual ASTKind GetKind() const;
		virtual WeakRef<const Token_t> GetToken() const;

		SY_PROPERTY_GET(GetKind) ASTKind Kind;
		SY_PROPERTY_GET(GetToken) WeakRef<const Token_t> Token;
	};

	class SYC_API MemberAST;
	class SYC_API FunctionAST;

	class SYC_API StatementAST;
}

#include "Symple/Code/AST/MemberAST.h"
#include "Symple/Code/AST/StatementAST.h"