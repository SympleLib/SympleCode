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
}