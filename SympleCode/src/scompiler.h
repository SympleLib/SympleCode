#pragma once

#include <string>
#include <vector>

#include "sserializer.hpp"

#define VAR 0x8A25E7BE
#define CALL 0xB3F184A9

#define CREATE_VAR ((char)0x00)
#define CALL_FUNC ((char)0x01)

namespace symple
{
	void CompileSource(const std::string&);
	serializer CompileLine(const std::string&);

	int ParseDigit(char);
	int ParseInt(const std::string&);
}