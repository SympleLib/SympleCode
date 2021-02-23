#pragma once

#include <iostream>

#include "SympleCode/Util/ConsoleColor.h"
#include "SympleCode/Syntax/Token.h"

namespace Symple::Syntax
{
	class Node
	{
	public: enum Kind : unsigned;
	protected:
		shared_ptr<Token> mToken;

		void PrintName(std::ostream& os = std::cout)
		{ os << KindMap[GetKind()] << "Syntax"; }
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
			PrintName(os);

			os.put('\n'); GetToken()->Print(os, std::string(indent) + GetAddIndent(last), true, "Token = ");
		}

		virtual void PrintShort(std::ostream& os)
		{ PrintName(os); os << ": "; GetToken()->PrintShort(os); }
		
		static void PrintIndent(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			Util::SetConsoleColor(Util::ConsoleColor::DarkGrey);
			
			os << indent;
			if (last)
				os << "L--";
			else
				os << "|--";

			Util::SetConsoleColor(Util::ConsoleColor::Cyan);
			os << label;
			
			Util::SetConsoleColor(Util::ConsoleColor::Green);
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

			TranslationUnit,

			Member,
			ExternFunction,
			GlobalStatement,
			FunctionDeclaration,
			StructDeclaration,


			Statement,
			Type,
			TypeReference,
			ImportStatement,
			NativeStatement,
			
			Label,
			IfStatement,
			GotoStatement,
			BlockStatement,
			ReturnStatement,
			ExpressionStatement,
			VariableDeclaration,


			Expression,
			UnaryExpression,
			BinaryExpression,
			LiteralExpression,
			ParenthesizedExpression,

			NameExpression,
			CallExpression,

			Last = CallExpression,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"TranslationUnit",

			"Member",
			"ExternFunction",
			"GlobalStatement",
			"FunctionDeclaration",
			"StructDeclaration",


			"Statement",
			"Type",
			"TypeReference",
			"ImportStatement",
			"NativeStatement",

			"Label",
			"IfStatement",
			"GotoStatement",
			"BlockStatement",
			"ReturnStatement",
			"ExpressionStatement",
			"VariableDeclaration",


			"Expression",
			"UnaryExpression",
			"BinaryExpression",
			"LiteralExpression",
			"ParenthesizedExpression",

			"NameExpression",
			"CallExpression",
		};
	};
}