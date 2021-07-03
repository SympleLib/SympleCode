#pragma once

#if defined(_MSC_VER)
#if !defined(SY_PROPERTY_GET)
	#define SY_PROPERTY_GET(getter) __declspec(property(get = getter))
	#define SY_PROPERTY_GET_SET(getter, setter) __declspec(property(get = getter, put = setter))
#endif
#else
#error Only MSVC supported for SympleCode
#endif

#if defined(SYC_BUILD_DLL)
	#define SYC_API __declspec(dllexport)
#elif defined(SYC_USE_DLL)
	#define SYC_API __declspec(dllimport)
#else
	#define SYC_API
#endif

#pragma warning(disable: 4244 4267)
namespace Symple::Code
{
	using int8 = char;
	using int16 = short;
	using int32 = int;
	using int64 = long long;

	using uint8 = unsigned char;
	using uint16 = unsigned short;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;
}

#define _CRT_SECURE_NO_WARNINGS
#include <cassert>
#include <string>
#include <sstream>
#include <vector>