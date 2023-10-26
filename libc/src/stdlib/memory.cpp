/*
* This implements the memory components of stdlib.h.
*/

#include <drivers/memory/memory.h>
#include <drivers/memory/physical_memory.h>

#include "stdlib.h"

using namespace slug_kernel::drivers::memory;

void* malloc(size_t size)
{
	if (size == 0)
		return nullptr;

	physical_memory_block* block = physical_memory_manager::find_free_block(size);

	if (block == nullptr)
		return nullptr;

	block->split(size);
	block->free = false;

	return block->get_address();
}

void free(void* ptr)
{
	if (ptr == 0)
		return;

	physical_memory_block* block = physical_memory_block::from_pointer(ptr);

	block->free = true;
	block->merge_previous();
	block->merge_next();

	if (block->previous)
		block->previous->merge_next();

	if (block->next)
		block->next->merge_previous();
}
