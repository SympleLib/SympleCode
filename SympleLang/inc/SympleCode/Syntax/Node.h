#pragma once

#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif

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
#ifdef _WIN32
			enum ConsoleColor
			{
				Black,
				DarkBlue,
				DarkGreen,
				DarkCyan,
				DarkRed,
				DarkMagenta,
				DarkYellow,
				Grey,
				DarkGrey,
				Blue,
				Green,
				Cyan,
				Red,
				Magenta,
				Yellow,
				White,

				Reset = White,
			};

			HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(console, DarkGrey);
#endif
			
			os << indent;
			if (last)
				os << "L--";
			else
				os << "|--";

			SetConsoleTextAttribute(console, Cyan);
			os << label;

#ifdef _WIN32
			SetConsoleTextAttribute(console, Green);
#endif
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
			GlobalStatement,
			FunctionDeclaration,


			Statement,
			Type,
			TypeReference,

			BlockStatement,
			ReturnStatement,
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
			"GlobalStatement",
			"FunctionDeclaration",


			"Statement",
			"Type",
			"TypeReference",
			"BlockStatement",
			"ReturnStatement",
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