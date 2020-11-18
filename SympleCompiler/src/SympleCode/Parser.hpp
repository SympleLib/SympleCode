#pragma once

#include <string_view>

#include "SympleCode/Tree/Tree.hpp"

namespace Symple::Parser
{
	void Parse(const char* source);

	Branch Walk();

	//Branch& ParseExpresion(const Type& type, const Token* beg, const Token* end);
#ifdef WIN32
	int32_t ParseInt(std::string_view view);
#else
	int64_t ParseInt(std::string_view view);
#endif
}