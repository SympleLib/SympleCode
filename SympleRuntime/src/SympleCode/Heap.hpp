#pragma once

#include <cstdint>

namespace Symple::Heap
{
	void SetSize(uint64_t size);

	uint64_t Alloc(uint64_t size);
	void Free(uint64_t ptr);

	void Move(uint64_t size, void* data, uint64_t ptr);
	uint8_t* Get(uint64_t size, uint64_t ptr);
}