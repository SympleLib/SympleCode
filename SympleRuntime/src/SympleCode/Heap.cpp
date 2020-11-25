#include <pch.hpp>
#include "SympleCode/Heap.hpp"

#include <vector>

namespace Symple::Heap
{
	std::vector<uint8_t*> sHeapPtrs;
	uint64_t sHeapSize;
	uint64_t sMaxHeapSize;

	void SetSize(uint64_t size)
	{
		sMaxHeapSize = size;
	}

	uint64_t Alloc(uint64_t size)
	{
		void* ptr = malloc(size);
		sHeapPtrs.push_back((uint8_t*)ptr);
		sHeapSize += size;
		assert(sHeapSize <= sMaxHeapSize); // Not enough heap space
		return sHeapPtrs.size() - 1;
	}

	void Free(uint64_t ptr)
	{
		free((void*)ptr);
	}

	void Move(uint64_t size, void* data, uint64_t ptr)
	{
		assert(sHeapSize >= ptr + size); // Data is not on stack.

		for (uint64_t i = 0; i < size; i++)
			sHeapPtrs[ptr][i] = ((uint8_t*)data)[i];
	}

	uint8_t* Get(uint64_t size, uint64_t ptr)
	{
		assert(sHeapSize >= ptr + size); // Data is not on stack.
		return sHeapPtrs[ptr];
	}
}