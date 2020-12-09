#pragma once

namespace Symple
{
	enum class LiteralType
	{
		Integer,
		LongInteger,
	};

	constexpr const int LiteralSizes[] = {
		4,
		8
	};

	constexpr const char* LiteralStrings[] = {
		"Integer",
		"LongInteger",
	};
}