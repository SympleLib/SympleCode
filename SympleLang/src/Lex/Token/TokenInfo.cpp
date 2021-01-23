#include <pch.h>
#include "SympleCode/Lex/Token/TokenInfo.h"

namespace Symple
{
	Ref<TokenInfo> TokenInfo::Default = MakeRef<TokenInfo>("<NA>", 0, 0);

	TokenInfo::TokenInfo(std::string_view file, unsigned line, unsigned column)
		: File(file), Line(line), Column(column)
	{}
}