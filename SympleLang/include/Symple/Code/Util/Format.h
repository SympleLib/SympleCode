#pragma once

#include <fmt/format.h>
#include <fmt/ostream.h>

namespace Symple::Code
{
	template <typename S, typename... Args>
	inline std::string Format(const S &fmt, Args&&... args)
	{ return fmt::format(fmt, args...); }
}