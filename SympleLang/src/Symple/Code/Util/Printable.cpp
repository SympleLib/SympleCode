#include "Symple/Code/Util/Printable.h"

#include <ostream>

#include "Symple/Code/Util/Console.h"

namespace Symple::Code
{
	void Printable::PrintIndent(OStream &os, StringView indent, StringView label, bool last)
	{
		Console.color = ConsoleColor::Grey;
		os << indent;
		os << (last ? "L--" : "|--");

		Console.color = ConsoleColor::Cyan;
		os << label;

		Console.color = ConsoleColor::Green;
	}

	const char *Printable::GetAddIndent(bool last)
	{ return (last ? "   " : "|  "); }

	void Printable::Print(OStream &os, StringView indent, StringView label, bool last) const
	{
		PrintIndent(os, indent, label, last);
		os << "Printable Object";
	}
}