#pragma once

#include "Symple/Code/Type/Type.h"
#include "Symple/Code/Parse/Token.h"
#include "Symple/Code/Util/Printable.h"

namespace Symple::Code
{
	class SYC_API Symbol
	{
	protected:
		std::string m_MangledName;
	public:
		const std::string &GetMangledName() const;

		SY_PROPERTY_GET(GetMangledName) const std::string &MangledName;
	};

	class SYC_API Ast;
	class SYC_API TypeAst;
	class SYC_API ParameterAst;
	class SYC_API CompilationUnitAst;

	class SYC_API MemberAst;
	class SYC_API FunctionAst;

	class SYC_API StatementAst;
	class SYC_API EmptyStatementAst;
	class SYC_API BlockStatementAst;
	class SYC_API ReturnStatementAst;
	class SYC_API ExpressionStatementAst;

	class SYC_API ExpressionAst;
	class SYC_API CastExpressionAst;
	class SYC_API CallExpressionAst;
	class SYC_API BinaryExpressionAst;
	class SYC_API LiteralExpressionAst;
	class SYC_API ParenthasizedExpressionAst;

	using ParameterList = std::vector<GlobalRef<ParameterAst>>;
	using ConstParameterList = std::vector<GlobalRef<const ParameterAst>>;

	using MemberList = std::vector<GlobalRef<MemberAst>>;
	using ConstMemberList = std::vector<GlobalRef<const MemberAst>>;

	using StatementList = std::vector<GlobalRef<StatementAst>>;
	using ConstStatementList = std::vector<GlobalRef<const StatementAst>>;

	using ExpressionList = std::vector<GlobalRef<ExpressionAst>>;
	using ConstExpressionList = std::vector<GlobalRef<const ExpressionAst>>;

	enum class SYC_API AstKind
	{
		Unknown,
		Type,
		Parameter,
		CompilationUnit,

		// Members
		Member,
		Function,

		// Statements
		Statement,
		EmptyStatement,
		BlockStatement,
		ReturnStatement,
		ExpressionStatement,

		// Expressions
		Expression,
		CastExpression,
		CallExpression,
		UnaryExpression,
		BinaryExpression,
		LiteralExpression,
		ParenthasizedExpression,

		Count, // Count of ast kinds
	};

	constexpr const char *const AstKindNames[(uint32)AstKind::Count] =
	{
		"Unknown",
		"Type",
		"Parameter",
		"CompilationUnit",


		"Member",
		"Function",


		"Statement",
		"EmptyStatement",
		"BlockStatement",
		"ReturnStatement",
		"ExpressionStatement",


		"Expression",
		"CastExpression",
		"CallExpression",
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

	class SYC_API TypeAst: public Ast, public Symbol
	{
	public:
		using Type_t = Type;
	private:
		WeakRef<const Token_t> m_Base;
		ConstWeakTokenList m_Addons;

		GlobalRef<Type_t> m_Type;

		friend class TypeVisitor;
	public:
		TypeAst(WeakRef<const Token_t> base, const ConstWeakTokenList &addons, GlobalRef<Type_t> type);

		virtual AstKind GetKind() const;
		virtual WeakRef<const Token_t> GetToken() const;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		WeakRef<const Token_t> GetBase() const;
		const ConstWeakTokenList &GetAddons() const;

		GlobalRef<const Type_t> GetType() const;

		SY_PROPERTY_GET(GetBase) WeakRef<const Token_t> Base;
		SY_PROPERTY_GET(GetAddons) const ConstWeakTokenList &Addons;
		SY_PROPERTY_GET(GetType) GlobalRef<const Type_t> Type;
	};


	class SYC_API ParameterAst: public Ast
	{
	private:
		GlobalRef<TypeAst> m_Type;
		GlobalRef<const Token_t> m_Name;

		friend class Parser;
	public:
		ParameterAst(const GlobalRef<TypeAst> &type, const GlobalRef<const Token_t> &name);

		virtual AstKind GetKind() const;
		virtual WeakRef<const Token_t> GetToken() const;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		GlobalRef<const TypeAst> GetType() const;
		GlobalRef<const Token_t> GetName() const;

		SY_PROPERTY_GET(GetType) GlobalRef<const TypeAst> Type;
		SY_PROPERTY_GET(GetName) GlobalRef<const Token_t> Name;
	};

	class SYC_API CompilationUnitAst: public Ast
	{
	private:
		MemberList m_Members;
		WeakRef<const Token_t> m_EndOfFile;

		friend class SymbolVisitor;
		friend class TypeVisitor;
	public:
		CompilationUnitAst(const MemberList &members, const WeakRef<const Token_t> &endOfFile);

		virtual AstKind GetKind() const override;
		virtual WeakRef<const Token_t> GetToken() const override;
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const override;

		const MemberList &GetMembers() const;
		WeakRef<const Token_t> GetEndOfFile() const;

		SY_PROPERTY_GET(GetMembers) const MemberList &Members;
		SY_PROPERTY_GET(GetEndOfFile) WeakRef<const Token_t> EndOfFile;
	};
}

#include "Symple/Code/Ast/MemberAst.h"
#include "Symple/Code/Ast/StatementAst.h"
#include "Symple/Code/Ast/ExpressionAst.h"