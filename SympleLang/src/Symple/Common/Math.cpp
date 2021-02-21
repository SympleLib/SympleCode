#include "Symple/Common/Math.h"

namespace Symple
{
	int32 Mathi::Align(int32 n, int32 a)
	{ return (n + a - 1) / a * a; }
}