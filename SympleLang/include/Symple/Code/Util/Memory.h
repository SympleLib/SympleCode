#pragma once

#include "Symple/Code/Common.h"
#include <memory>

namespace Symple::Code
{
	template<typename T>
	struct Ref
	{
		T *const ptr;

		Ref() = default;
		Ref(const T *ptr)
			: ptr(const_cast<T *>(ptr))
		{ refCount++; }

		Ref(const Ref &other)
			: ptr(other.ptr)
		{ refCount++; }

		~Ref()
		{
			refCount--;
			if (refCount == 0)
				delete ptr;
		}

		T *operator ->()
		{ return ptr; }

		const T *operator ->() const
		{ return ptr; }
	private:
		static mutable Size refCount;
	};
}