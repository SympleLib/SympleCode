#pragma once

#if defined(SY_BUILD_DLL)
	#define SY_API __declspec(dllexport)
#elif defined(SY_BUILD_LIB)
	#define SY_API
#else
	#define SY_API __declspec(dllimport)
#endif