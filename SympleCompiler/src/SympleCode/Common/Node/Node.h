#pragma once

#include <list>
#include <cstdio>
#include <cassert>
#include <sstream>

namespace Symple
{
	class Node
	{
	public:
		enum class Kind
		{
			Unknown,

			Expression,
			UnaryExpression,
			FieldExpression,
			BinaryExpression,
			LiteralExpression,
			VariableExpression,
			OperatorExpression,
			AssignmentExpression,
			ModifiableExpression,
			FunctionCallExpression,
			PointerIndexExpression,
			NumberLiteralExpression,
			StringLiteralExpression,
			BooleanLiteralExpression,

			Member,
			CompilationUnit,
			FunctionDeclaration,

			Statement,
			BlockStatement,
			BreakStatement,
			WhileStatement,
			GlobalStatement,
			ReturnStatement,
			VariableDeclaration,
			ExpressionStatement,

			FunctionHint,
			ExternFunction,
			FunctionArgument,
			FunctionArguments,
			FunctionCallArguments,
		};

		static constexpr const char* KindMap[] = {
			"Unknown", "Expression", "UnaryExpression", "BinaryExpression",
			"FieldExpression",
			"LiteralExpression", "VariableExpression", "OperatorExpression", "AssignmentExpression", "ModifiableExpression",
			"FunctionCallExpression", "PointerIndexExpression",
			"StringLiteralExpression", "NumberLiteralExpression", "BooleanLiteralExpression",
			"Member", "CompilationUnit", "FunctionDeclaration",
			"Statement", "BlockStatement", "BreakStatement", "WhileStatement", "GlobalStatement", "ReturnStatement", "VariableDeclaration", "ExpressionStatement",
			"FunctionHint", "ExternFunction", "FunctionArgument", "FunctionArguments", "FunctionCallArguments",
		};

		static constexpr const char* KindString(Kind kind)
		{
			return KindMap[(int)kind];
		}

		template<typename T>
		bool Is() const
		{
			return dynamic_cast<const T*>(this);
		}

		template<typename T>
		T* Cast()
		{
			return dynamic_cast<T*>(this);
		}

		template<typename T>
		const T* Cast() const
		{
			return dynamic_cast<const T*>(this);
		}

		virtual Kind GetKind() const
		{
			return Kind::Unknown;
		}

		virtual std::string ToString(const std::string& indent = "", bool last = true) const
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Node of Kind: " << KindString(GetKind());
			return ss.str();
		}
	};
}