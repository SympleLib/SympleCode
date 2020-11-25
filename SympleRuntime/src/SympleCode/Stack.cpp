#include "SympleCode/Stack.hpp"

#include <cassert>

namespace Symple::Stack
{
	uint8_t* sStack;
	uint64_t sStackSize;
	uint64_t sStackPtr;

	void SetSize(uint64_t size)
	{
		delete[] sStack;
		sStack = new uint8_t[sStackSize = size];
	}

	uint64_t Alloc(uint64_t mem)
	{
		uint64_t ptr = sStackPtr;
		sStackPtr += mem;
		assert(sStackPtr <= sStackSize); // Stack overflow
		return ptr;
	}

	void Free(uint64_t mem)
	{
		sStackPtr -= mem;
		assert(sStackPtr > 0);
	}

	void Move(uint64_t size, void* data, uint64_t pos)
	{
		assert(sStackPtr >= pos + size); // Data is not on stack.

		for (uint64_t i = 0; i < size; i++)
			sStack[pos + i] = ((uint8_t*)data)[i];
	}

	uint8_t* Get(uint64_t size, uint64_t pos)
	{
		assert(sStackPtr >= pos + size); // Data is not on stack.
		return &sStack[pos];
	}
}