/*
* This file implements types to represent Grand Descriptor Table related types in a convenient way.
*/

#include <stdint.h>

#include "arch/x86_64/gdt.h"

namespace slug_kernel::arch::x86_64::gdt
{
	gdt_entry::gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags)
	{
		base_high = (base & 0xFF000000) >> 24;
		flags_limit_high = (flags & 0x0F) << 4 | (limit & 0x000F0000) >> 16;
		access_byte = access;
		base_middle = (base & 0x00FF0000) >> 16;
		base_low = (base & 0x0000FFFF);
		limit_low = (limit & 0x0000FFFF);
	}

	gdt_pointer::gdt_pointer(gdt_entry* base, uint16_t size)
		: size(size),
		base(reinterpret_cast<uint64_t>(base))
	{}
}
