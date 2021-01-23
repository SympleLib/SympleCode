#pragma once

#include <string_view>

#include "SympleCode/Ref.h"

namespace Symple
{
	struct TokenInfo
	{
		static Ref<TokenInfo> Default;

		std::string_view File;
		unsigned Line, Column;

		TokenInfo(std::string_view file = Default->File, unsigned line = Default->Line, unsigned column = Default->Column);
	};
}