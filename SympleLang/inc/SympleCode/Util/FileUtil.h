#pragma once

#include <cstdio>
#include <string_view>

namespace Symple
{
	void OpenFile(FILE*&, std::string_view path);
	void CloseFile(FILE*);
}