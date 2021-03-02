#pragma once

#include "Symple/Code/Parse/Token.h"

namespace Symple::Code
{
	enum class SYC_API ASTKind
	{
		Unknown,

		// Members
		Member,

		Count, // Count of ast kinds
	};

	constexpr const char *const ASTKindNames[(uint32)ASTKind::Count] =
	{
		"Unknown",


		"Member",
	};

	class SYC_API AST
	{
	protected:
		using Token_t = Token;
	public:
		virtual ASTKind GetKind() const;
		virtual WeakRef<const Token_t> GetToken() const;

		SY_PROPERTY_GET(GetKind) ASTKind Kind;
		SY_PROPERTY_GET(GetToken) WeakRef<const Token_t> Token;
	};
}

#include "Symple/Code/AST/MemberAST.h"
#include "Symple/Code/AST/StatementAST.h"