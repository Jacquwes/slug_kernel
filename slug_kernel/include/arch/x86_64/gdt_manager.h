#pragma once

/*
* This file defines types to setup the gdt.
*/

#include <stdint.h>

#include "arch/x86_64/gdt.h"

namespace slug_kernel::arch::x86_64::gdt
{
	struct gdt_manager
	{
		static void init();
		static void load();

		static void set_entry(uint8_t index, uint64_t base, uint64_t limit, uint8_t access, uint8_t flags);

		static void flush();

		static gdt_entry entries[5];

		static gdt_pointer ptr;
	};
}
