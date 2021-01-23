#include <pch.h>
#include "SympleCode/Lex/Token/TokenKind.h"

namespace Symple
{
	char* TokenKindNames[(unsigned)TokenKind::Last + 1] = {
	   "EndOfFile",
	   "Unkown",

	   "Identifier",
	   "Number",
	};
}