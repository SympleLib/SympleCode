#pragma once

#include <string>
#include <typeinfo>
#include "Symple/Code/Common.h"

namespace Symple::Code
{
	template<typename T>
	inline std::string ToString(const T &item)
	{ return typeid(item).name() + std::to_string(&item); }

	std::string ToString(const std::string &item);
	std::string ToString(const char *const &item);
	std::string ToString(const void *const &item);
	std::string ToString(const uint32 &item);
	std::string ToString(const int &item);


	template<typename... Args>
	inline std::string Format(std::string_view fmt, Args&&... args)
	{

	}
}