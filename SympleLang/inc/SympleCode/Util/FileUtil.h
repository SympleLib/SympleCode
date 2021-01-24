#pragma once

#include <cstdio>
#include <string_view>

namespace Symple
{
	void OpenFile(FILE*&, std::string_view path);
	char* ReadFile(FILE*);
	void CloseFile(FILE*);
}