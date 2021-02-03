#pragma once

#include "SympleCode/Memory.h"

namespace Symple::Binding
{
	class BoundConstant
	{
	private:
		const type_info* mType;
		char mValue[64];
	public:
		template <typename T>
		T& GetValue()
		{ return *(T*)mValue; }
	};
}