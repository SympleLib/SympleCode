#include "Symple/Common/Test.h"

#include <cstdio>

SY_API void Test()
{
	puts("Test failed successfully");
#pragma warning(disable: 6031)
	getchar();
}