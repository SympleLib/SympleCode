#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple::Binding
{
	class Node
	{
	public: enum Kind : unsigned;
	protected:
		shared_ptr<Syntax::Node> mSyntax;

		void PrintName(std::ostream& os = std::cout)
		{ os << "Bound" << KindMap[GetKind()]; }
	public:
		static void PrintIndent(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{ return Syntax::Node::PrintIndent(os, indent, last, label); }
		static char* GetAddIndent(bool last = true)
		{ return Syntax::Node::GetAddIndent(last); }
	public:
		Node(shared_ptr<Syntax::Node> syntax)
			: mSyntax(syntax) {}

		bool Is(Kind kind)
		{ return GetKind() == kind; }
		template <typename... Args>
		bool Is(Kind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		virtual Kind GetKind()
		{ return Unknown; }

		shared_ptr<Syntax::Node> GetSyntax()
		{ return mSyntax; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os.put('\n'); GetSyntax()->Print(os, std::string(indent) + GetAddIndent(last), true, "Syntax = ");
		}
	public:
		enum Kind : unsigned
		{
			Unknown,

			CompilationUnit,

			Member,
			Function,

			Statement,
			NativeCode,
			BlockStatement,
			ReturnStatement,
			ExpressionStatement,
			VariableDeclaration,

			Expression,
			ErrorExpression,
			UnaryExpression,
			BinaryExpression,
			LiteralExpression,
			AssignmentExpression,

			DefaultExpression,
			ConstantExpression,
			ImplicitCastExpression,

			CallExpression,
			FunctionPointer,
			VariableExpression,

			Last = VariableExpression,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"CompilationUnit",

			"Member",
			"Function",

			"Statement",
			"NativeCode",
			"BlockStatement",
			"ReturnStatement",
			"ExpressionStatement",
			"VariableDeclaration",

			"Expression",
			"ErrorExpression",
			"UnaryExpression",
			"BinaryExpression",
			"LiteralExpression",
			"AssignmentExpression",
			
			"DefaultExpression",
			"ConstantExpression",
			"ImplicitCastExpression",

			"CallExpression",
			"FunctionPointer",
			"VariableExpression",
		};
	};
}