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
		static void flush();

		static void display_entries();

		static gdt_entry entries[4];

		static gdt_pointer ptr;
	};
}
