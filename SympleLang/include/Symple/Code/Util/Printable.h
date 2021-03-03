#pragma once

#include <string>

namespace Symple::Code
{
	class Printable
	{
	public:
		static void PrintIndent(std::ostream &, std::string_view indent = "", std::string_view label = "", bool last = true);
		static const char *GetAddIndent(bool last = true);
		virtual void Print(std::ostream &, std::string indent = "", std::string_view label = "", bool last = true) const;
	};
}