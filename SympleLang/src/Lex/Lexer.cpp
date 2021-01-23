#include <pch.h>
#include "SympleCode/Lex/Lexer.h"

#include <cstring>

namespace Symple
{
	Lexer::Lexer(std::string_view path)
		: mPath(path), mLine(), mColumn()
	{

	}
}