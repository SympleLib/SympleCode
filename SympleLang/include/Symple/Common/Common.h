#pragma once

#if defined(SY_BUILD_DLL)
	#define SY_API __declspec(dllexport)
#elif defined(SY_BUILD_LIB)
	#define SY_API
#else
	#define SY_API __declspec(dllimport)
#endif

using int8  = char;
using int16 = short;
using int32 = int;
using int64 = long long;

using uint8  = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

#include <string>
#include <sstream>

#include "Symple/Common/Memory.h"