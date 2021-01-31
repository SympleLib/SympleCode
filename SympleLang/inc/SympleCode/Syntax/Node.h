#pragma once

#include <iostream>

#include "SympleCode/Syntax/Token.h"

namespace Symple::Syntax
{
	class Node
	{
	public: enum Kind : unsigned;
	protected:
		shared_ptr<Token> mToken;
	public:
		Node(shared_ptr<Token> tok)
			: mToken(tok) {}

		bool Is(Kind kind)
		{ return GetKind() == kind; }
		template <typename... Args>
		bool Is(Kind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		virtual Kind GetKind()
		{ return Unknown; }

		shared_ptr<Token> GetToken()
		{ return mToken; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << KindMap[GetKind()] << " Syntax";
		}
		
		static void PrintIndent(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			os << indent;
			if (last)
				os << "L--";
			else
				os << "|--";
			os << label;
		}

		static char* GetAddIndent(bool last = true)
		{
			if (last)
				return "    ";
			else
				return "|   ";
		}
	public:
		enum Kind : unsigned
		{
			Unknown,

			Expression,
			UnaryExpression,
			BinaryExpression,
			LiteralExpression,
			ParenthesizedExpression,

			Last = ParenthesizedExpression,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"Expression",
			"UnaryExpression",
			"BinaryExpression",
			"LiteralExpression",
			"ParenthesizedExpression",
		};
	};
}