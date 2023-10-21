/*
* This file implements types to represent memory in a convenient way.
*/

#include <stdint.h>

#include "drivers/memory/memory.h"

namespace slug_kernel::drivers::memory
{
	bool physical_memory_block::merge_previous()
	{
		if (!free)
			return false;

		if (!previous || !previous->free)
			return false;

		if (previous->get_end_address() != get_address())
			return false;

		previous->size += size + sizeof(physical_memory_block);
		previous->next = next;

		if (next)
			next->previous = previous;

		return true;
	}

	bool physical_memory_block::merge_next()
	{
		if (!free)
			return false;

		if (!next || !next->free)
			return false;

		if (get_end_address() != next->get_address())
			return false;

		size += next->size + sizeof(physical_memory_block);
		next = next->next;

		if (next)
			next->previous = this;

		return true;
	}

	bool physical_memory_block::split(uint64_t const& size)
	{
		if (!free)
			return false;

		if (this->size < size + sizeof(physical_memory_block))
			return false;

		physical_memory_block* new_block = reinterpret_cast<physical_memory_block*>(reinterpret_cast<uint64_t>(this) + size + sizeof(physical_memory_block));

		new_block->free = true;
		new_block->size = this->size - size - sizeof(physical_memory_block);
		new_block->previous = this;
		new_block->next = next;

		if (next)
			next->previous = new_block;

		this->size = size;
		this->next = new_block;

		return true;
	}

	constexpr void* physical_memory_block::get_address() const
	{
		return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(this) + sizeof(physical_memory_block));
	}

	constexpr void* physical_memory_block::get_end_address() const
	{
		return reinterpret_cast<void*>(reinterpret_cast<uint64_t>(this) + sizeof(physical_memory_block) + size);
	}

	constexpr uint64_t physical_memory_block::get_allocated_size() const
	{
		return size - sizeof(physical_memory_block);
	}

	physical_memory_block* physical_memory_block::from_pointer(void* address)
	{
		return reinterpret_cast<physical_memory_block*>(reinterpret_cast<uint64_t>(address) - sizeof(physical_memory_block));
	}
}
