#include "Symple/Code/Util/Printable.h"

#include <ostream>

#include "Symple/Code/Util/Console.h"

namespace Symple::Code
{
	void Printable::PrintIndent(std::ostream &os, std::string_view indent, std::string_view label, bool last)
	{
		Console.Color = ConsoleColor::Grey;
		os << indent;
		os << (last ? "L--" : "|--");

		Console.Color = ConsoleColor::Cyan;
		os << label;

		Console.Color = ConsoleColor::Green;
	}

	const char *Printable::GetAddIndent(bool last)
	{ return (last ? "   " : "|  "); }

	void Printable::Print(std::ostream &os, std::string indent, std::string_view label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		os << "Printable Object";
	}
}