/*
* This file implements types to setup the gdt.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "arch/x86_64/gdt.h"
#include "arch/x86_64/gdt_manager.h"

namespace slug_kernel::arch::x86_64::gdt
{
	gdt_pointer gdt_manager::ptr = gdt_pointer(entries, 4);
	gdt_entry gdt_manager::entries[4];

	void gdt_manager::init()
	{
		flush();

		uint32_t base = 0;
		uint32_t limit = 0;
		uint8_t access = 0;
		uint8_t flags = 0;

		// Null segment
		entries[0] = gdt_entry(0, 0, 0, 0);

		// Kernel code segment
		access = static_cast<uint8_t>(access_byte::present::yes) | 
			static_cast<uint8_t>(access_byte::privilege::ring0) | 
			static_cast<uint8_t>(access_byte::descriptor_type::code_or_data) | 
			static_cast<uint8_t>(access_byte::executable::yes) | 
			static_cast<uint8_t>(access_byte::conforming::no) | 
			static_cast<uint8_t>(access_byte::readable::yes);

		flags = static_cast<uint8_t>(flags::granularity::page) |
			static_cast<uint8_t>(flags::size::_64bit) |
			static_cast<uint8_t>(flags::long_mode::yes);

		entries[1] = gdt_entry(base, limit, access, flags);

		// Kernel data segment
		access = static_cast<uint8_t>(access_byte::present::yes) | 
			static_cast<uint8_t>(access_byte::privilege::ring0) | 
			static_cast<uint8_t>(access_byte::descriptor_type::code_or_data) | 
			static_cast<uint8_t>(access_byte::executable::no) | 
			static_cast<uint8_t>(access_byte::direction::grows_up) | 
			static_cast<uint8_t>(access_byte::writable::yes);

		flags = static_cast<uint8_t>(flags::granularity::page) |
			static_cast<uint8_t>(flags::size::_64bit) |
			static_cast<uint8_t>(flags::long_mode::yes);

		entries[2] = gdt_entry(base, limit, access, flags);

		// TODO: task state segment
		task_state_segment tss;
		base = reinterpret_cast<uint64_t>(&tss);
		limit = sizeof(task_state_segment);
		access = 0x89;
		flags = 0x00;
		entries[3] = gdt_entry(base, limit, access, flags);
	}

	void gdt_manager::load()
	{
		asm volatile ("lgdt %0" : : "m" (ptr));
	}

	void gdt_manager::flush()
	{
		for (size_t i = 0; i < 3; i++)
		{
			entries[i] = gdt_entry(0, 0, 0, 0);
		}
	}

	void gdt_manager::display_entries()
	{
		printf("--------------------\n"
			"Grand Descriptor Table:\n");

		for (size_t i = 0; i < 4; i++)
		{
			uint32_t base = entries[i].base_high << 24 | entries[i].base_middle << 16 | entries[i].base_low;
			uint32_t limit = (entries[i].flags_limit_high & 0x0F) << 16 | entries[i].limit_low;
			uint8_t access = entries[i].access_byte;
			uint8_t flags = entries[i].flags_limit_high >> 4;

			printf("Base: %x, limit: %x, access byte: %x, flags: %x\n", base, limit, access, flags);
		}

		printf("--------------------\n");
	}
}
