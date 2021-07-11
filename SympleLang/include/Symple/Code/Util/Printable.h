#pragma once

#include "Symple/Code/Common.h"

namespace Symple::Code
{
	class Printable
	{
	public:
		static void PrintIndent(OStream &, StringView indent = "", StringView label = "", bool last = true);
		static const char *GetAddIndent(bool last = true);
		virtual void Print(OStream &, StringView indent = "", StringView label = "", bool last = true) const;
	};
}