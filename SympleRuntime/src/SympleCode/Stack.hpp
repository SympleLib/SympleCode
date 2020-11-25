#pragma once

#include <cstdint>

namespace Symple::Stack
{
	void SetSize(uint64_t size);

	uint64_t Alloc(uint64_t mem);
	void Free(uint64_t mem);

	void Move(uint64_t size, void* data, uint64_t pos);
	uint8_t* Get(uint64_t size, uint64_t pos);
}