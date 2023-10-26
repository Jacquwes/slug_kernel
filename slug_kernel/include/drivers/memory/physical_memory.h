#pragma once

/*
* This file defines types to manage memory in a convenient way.
*/

#include <stddef.h>
#include <stdint.h>

#include "drivers/memory/memory.h"

namespace slug_kernel::drivers::memory
{
	class physical_memory_manager
	{
	public:
		static bool init();

		static physical_memory_block* find_free_block(size_t size);

		static void display_memory_map();

	private:
		static physical_memory_block* first_block;
	};
}
