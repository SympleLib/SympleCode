#include "Symple/Code/Util/Format.h"

namespace Symple::Code
{
	std::string ToString(const std::string &item)
	{ return item; }

	std::string ToString(const char *const &item)
	{ return item; }

	std::string ToString(const void *const &item)
	{ return std::to_string((uint32)item); }

	std::string ToString(const uint32 &item)
	{ return std::to_string(item); }

	std::string ToString(const int &item)
	{ return std::to_string(item); }
}