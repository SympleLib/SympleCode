#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple::Symbol
{
	class Symbol
	{
	public: enum Kind : unsigned;
	protected:
		void PrintName(std::ostream& os = std::cout)
		{ os << KindMap[GetKind()] << "Symbol"; }
	public:
		static void PrintIndent(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{ return Syntax::Node::PrintIndent(os, indent, last, label); }
		static char* GetAddIndent(bool last = true)
		{ return Syntax::Node::GetAddIndent(last); }
	public:
		bool Is(Kind kind)
		{ return GetKind() == kind; }
		template <typename... Args>
		bool Is(Kind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		virtual Kind GetKind()
		{ return Unknown; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
		}
	public:
		enum Kind : unsigned
		{
			Unknown,

			Type,
			StructType,
			Member,

			Label,
			Function,
			Variable,
			Parameter,

			Last = Parameter,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"Type",
			"StructType",
			"Member",

			"Label",
			"Function",
			"Variable",
			"Parameter",
		};
	};
}