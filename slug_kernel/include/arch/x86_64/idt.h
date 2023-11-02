#pragma once

/*
* This file defines types to represent Interrupt Descriptor Table related types in a convenient way.
*/

#include <stdint.h>

namespace slug_kernel::arch::x86_64::idt
{
	struct segment_selector
	{
		uint16_t value;

		constexpr segment_selector() = default;

		segment_selector(uint16_t index, uint8_t table, uint8_t privilege_level);
	} __attribute__((packed));

	struct entry_flags
	{
		uint8_t value;

		constexpr entry_flags() = default;

		entry_flags(gate_type gate_type, privilege_level privilege_level);
	} __attribute__((packed));

	struct idt_entry
	{
		uint16_t offset_low;
		segment_selector selector;
		uint8_t ist;
		entry_flags flags;
		uint16_t offset_middle;
		uint32_t offset_high;
		uint32_t reserved;

		constexpr idt_entry() = default;

		idt_entry(uint64_t offset, segment_selector selector, uint8_t ist, entry_flags flags);
	};

	struct idt_pointer
	{
		uint16_t size;
		uint64_t base;

		constexpr idt_pointer() = default;

		idt_pointer(idt_entry* base, uint16_t size);
	} __attribute__((packed));

	enum class privilege_level : uint8_t
	{
		ring0 = 0b00,
		ring1 = 0b01,
		ring2 = 0b10,
		ring3 = 0b11
	};

	enum class gate_type
	{
		interrupt_gate = 0xE,
		trap_gate = 0xF
	};
}