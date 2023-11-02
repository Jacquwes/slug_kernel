/*
* This file implements types to represent Interrupt Descriptor Table related types in a convenient way.
*/

#include <stdint.h>

#include "arch/x86_64/idt.h"

namespace slug_kernel::arch::x86_64::idt
{
	segment_selector::segment_selector(uint16_t index, uint8_t table, uint8_t privilege_level)
	{
		value = (index & 0b0001'1111) << 3;
		value |= (table & 0b0000'0001) << 2;
		value |= privilege_level & 0b0000'0011;
	}

	entry_flags::entry_flags(gate_type gate_type, privilege_level privilege_level)
	{
		value = 0b1000'0000;
		value |= (static_cast<uint8_t>(privilege_level) & 0b0000'0011) << 5;
		value |= static_cast<uint8_t>(gate_type) & 0b0000'1111;
	}

	idt_entry::idt_entry(uint64_t offset, segment_selector selector, uint8_t ist, entry_flags flags)
	{
		offset_high = (offset & 0xFFFF'FFFF'0000'0000) >> 32;
		offset_middle = (offset & 0x0000'0000'FFFF'0000) >> 16;
		this->flags = flags;
		this->ist = ist & 0b0000'0111;
		this->selector = selector;
		offset_low = offset & 0x0000'0000'0000'FFFF;
	}
}
