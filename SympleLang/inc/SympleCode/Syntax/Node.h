#pragma once

#include <iostream>

#include "SympleCode/Syntax/Token.h"

namespace Symple
{
	class Node
	{
	public: enum Kind : unsigned;
	protected:
		std::shared_ptr<Token> mToken;

		void PrintIndent(std::ostream& os = std::cout, std::string_view indent = "", bool last = true)
		{
			os << indent;
			if (last)
				os << "L--\t";
			else
				os << "|--\t";
		}

		char* GetAddIndent(bool last = true)
		{
			if (last)
				return " \t";
			else
				return "|\t";
		}
	public:
		Node(std::shared_ptr<Token> tok)
			: mToken(tok) {}

		virtual Kind GetKind()
		{ return Unknown; }

		virtual std::shared_ptr<Token> GetToken()
		{ return mToken; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true)
		{
			PrintIndent(os, indent, last);
			os << "Node of Kind: " << KindMap[GetKind()];
		}

		template <typename T>
		std::shared_ptr<T> As()
		{ return std::shared_ptr<T>(this); }
	public:
		enum Kind : unsigned
		{
			Unknown,

			Expression,
			LiteralExpression,

			Last = LiteralExpression,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"Expression",
			"LiteralExpression",
		};
	};
}