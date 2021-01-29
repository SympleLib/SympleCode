#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple::Binding
{
	class Node
	{
	public: enum Kind : unsigned;
	protected:
		shared_ptr<Syntax::Node> mSyntax;
		
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

		virtual void Print(std::ostream & os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Bound " << KindMap[GetKind()];
		}

		template <typename T>
		shared_ptr<T> As()
		{ return shared_ptr<T>(this); }
	public:
		enum Kind : unsigned
		{
			Unknown,

			Expression,
			ErrorExpression,
			BinaryExpression,

			Last = BinaryExpression,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"Expression",
			"ErrorExpression",
			"BinaryExpression",
		};
	};
}