#pragma once

#include "Symple/Common/Common.h"

namespace Symple
{
	struct Mathi
	{
		Mathi() = delete;

		static int32 Align(int32 number, int32 align);
	};
}