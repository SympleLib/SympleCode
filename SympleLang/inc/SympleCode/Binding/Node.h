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

		virtual Kind GetKind()
		{ return Unknown; }

		shared_ptr<Syntax::Node> GetSyntax()
		{ return mSyntax; }

		virtual void Print(std::ostream & os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << KindMap[GetKind()] << " Node";
		}
	public:
		enum Kind : unsigned
		{
			Unknown,

			Last = Unknown,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",
		};
	};
}