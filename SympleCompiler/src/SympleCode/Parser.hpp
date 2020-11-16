#pragma once

#include <string_view>

namespace Symple::Parser
{
	void Parse(const char*);

#ifdef WIN32
	int32_t ParseInt(std::string_view view);
#else
	int64_t ParseInt(std::string_view view);
#endif
}