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

		void PrintIndent(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			os << indent;
			if (last)
				os << "L--";
			else
				os << "|--";
			os << label;
		}

		char* GetAddIndent(bool last = true)
		{
			if (last)
				return "    ";
			else
				return "|   ";
		}
	public:
		Node(shared_ptr<Token> tok)
			: mToken(tok) {}

		virtual Kind GetKind()
		{ return Unknown; }

		shared_ptr<Token> GetToken()
		{ return mToken; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << KindMap[GetKind()] << " Syntax";
		}

		template <typename T>
		shared_ptr<T> As()
		{ return shared_ptr<T>(this); }
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