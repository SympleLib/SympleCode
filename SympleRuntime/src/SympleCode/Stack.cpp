#include <pch.hpp>
#include "SympleCode/Stack.hpp"

namespace Symple::Stack
{
	uint8_t* sStack = nullptr;
	uint64_t sStackSize;
	uint64_t sStackPtr;

	void SetSize(uint64_t size)
	{
		delete[] sStack;
		sStack = new uint8_t[sStackSize = size];
	}

	uint64_t Alloc(uint64_t size)
	{
		uint64_t ptr = sStackPtr;
		sStackPtr += size;
		assert(sStackPtr <= sStackSize); // Stack overflow
		return ptr;
	}

	void Free(uint64_t size)
	{
		sStackPtr -= size;
		assert(sStackPtr >= 0);
	}

	void Move(uint64_t size, void* data, uint64_t ptr)
	{
		assert(sStackPtr >= ptr + size); // Data is not on stack.

		for (uint64_t i = 0; i < size; i++)
			sStack[ptr + i] = ((uint8_t*)data)[i];
	}

	uint8_t* Get(uint64_t size, uint64_t ptr)
	{
		assert(sStackPtr >= ptr + size); // Data is not on stack.
		return &sStack[ptr];
	}
}