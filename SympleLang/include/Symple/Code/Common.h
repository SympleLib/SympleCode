#pragma once

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
	
	using it = size_t;
	using Size = size_t;
}