#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <limine.h>

#include "drivers/memory/memory.h"
#include "drivers/memory/physical_memory.h"

namespace slug_kernel::drivers::memory
{
	static volatile limine_memmap_request memory_map_request =
	{
		.id = LIMINE_MEMMAP_REQUEST,
		.revision = 0,
		.response = 0,
	};

	physical_memory_block* physical_memory_manager::first_block = nullptr;

	bool physical_memory_manager::init()
	{
		limine_memmap_response* response = memory_map_request.response;

		if (!response)
			return false;

		if (!response->entry_count)
			return false;

		physical_memory_block* current_block = nullptr;

		for (uint64_t entry_i = 0; entry_i < response->entry_count; entry_i++)
		{
			limine_memmap_entry* entry = response->entries[entry_i];

			if (entry->type != LIMINE_MEMMAP_USABLE)
				continue;

			if (!current_block)
			{
				current_block = reinterpret_cast<physical_memory_block*>(entry->base);
				current_block->free = true;
				current_block->size = entry->length;
				current_block->previous = nullptr;
				current_block->next = nullptr;

				first_block = current_block;
			}
			else
			{
				current_block->next = reinterpret_cast<physical_memory_block*>(entry->base);
				current_block->next->free = true;
				current_block->next->size = entry->length;
				current_block->next->previous = current_block;
				current_block->next->next = nullptr;

				current_block = current_block->next;
			}
		}

		return true;
	}

	physical_memory_block* physical_memory_manager::find_free_block(size_t size)
	{
		auto current_block = first_block;

		while (current_block)
		{
			if (current_block->free && current_block->size >= size)
				return current_block;

			current_block = current_block->next;
		}

		return nullptr;
	}

	void physical_memory_manager::display_memory_map()
	{
		printf("--------------------\n"
			"Memory map:\n");

		auto current_block = first_block;

		while (current_block)
		{
			printf(
				"Block at %p, size %x, free: %d next: %p, previous: %p\n",
				current_block, current_block->size + sizeof(physical_memory_block), current_block->free,
				current_block->next, current_block->previous);

			current_block = current_block->next;
		}

		printf("--------------------\n");
	}
}
