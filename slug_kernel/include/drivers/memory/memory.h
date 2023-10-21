#pragma once

/*
* This file defines types to represent memory in a convenient way.
*/

#include <stdint.h>

namespace slug_kernel::drivers::memory
{
	struct physical_memory_block
	{
		bool free;

		uint64_t size;

		physical_memory_block* previous;
		physical_memory_block* next;

		physical_memory_block() = default;

		// This function returns false if memory is unchanged.
		bool merge_previous();

		// This function returns false if memory is unchanged.
		bool merge_next();

		// This function returns false if the block is not free or if size is too big.
		bool split(uint64_t const& size);

		constexpr void* get_address() const;
		constexpr void* get_end_address() const;
		constexpr uint64_t get_allocated_size() const;
	};
}
