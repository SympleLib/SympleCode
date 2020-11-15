#pragma once

#include <sstream>

namespace Parser
{
	void Parse(const char*);

#ifdef WIN32
	int32_t ParseInt(const std::string_view& view);
#else
	int64_t ParseInt(const std::string_view& view);
#endif

	int powi(int, int);
}