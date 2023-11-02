#pragma once

/*
* This file defines types to represent Grand Descriptor Table related types in a convenient way.
*/

#include <stdint.h>

namespace slug_kernel::arch::x86_64::gdt
{
	struct gdt_entry
	{
		uint16_t limit_low;
		uint16_t base_low;
		uint8_t base_middle;
		uint8_t access_byte;
		uint8_t flags_limit_high;
		uint8_t base_high;

		constexpr gdt_entry() = default;

		gdt_entry(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags);
	} __attribute__((packed));

	struct gdt_pointer
	{
		uint16_t size;
		uint64_t base;

		constexpr gdt_pointer() = default;

		gdt_pointer(gdt_entry* base, uint16_t size);
	} __attribute__((packed));

	namespace flags
	{
		enum class granularity : uint8_t
		{
			byte = 0b0000,
			page = 0b1000
		};

		enum class size : uint8_t
		{
			_16bit = 0b0000,
			_32bit = 0b0100,
			_64bit = 0b0000,
		};

		enum class long_mode
		{
			no = 0b0000,
			yes = 0b0010
		};
	}

	namespace access_byte
	{
		enum class present : uint8_t
		{
			no = 0b0000'0000,
			yes = 0b1000'0000
		};

		enum class privilege : uint8_t
		{
			ring0 = 0b0000'0000,
			ring1 = 0b0010'0000,
			ring2 = 0b0100'0000,
			ring3 = 0b0110'0000
		};

		enum class descriptor_type : uint8_t
		{
			system = 0b0000'0000,
			code_or_data = 0b0001'0000
		};

		enum class executable : uint8_t
		{
			no = 0b0000'0000,
			yes = 0b0000'1000
		};

		enum class direction : uint8_t
		{
			grows_up = 0b0000'0000,
			grows_down = 0b000'00100
		};

		enum class conforming : uint8_t
		{
			no = 0b0000'0000,
			yes = 0b0000'0010
		};

		enum class readable : uint8_t
		{
			no = 0b0000'0000,
			yes = 0b0000'0010
		};

		enum class writable : uint8_t
		{
			no = 0b0000'0000,
			yes = 0b0000'0010
		};

		enum class accessed : uint8_t
		{
			no = 0b0000'0000,
			yes = 0b0000'0001
		};
	}

	struct task_state_segment
	{
		uint32_t reserved0;
		uint64_t rsp0;
		uint64_t rsp1;
		uint64_t rsp2;
		uint64_t reserved1;
		uint64_t ist1;
		uint64_t ist2;
		uint64_t ist3;
		uint64_t ist4;
		uint64_t ist5;
		uint64_t ist6;
		uint64_t ist7;
		uint64_t reserved2;
		uint16_t reserved3;
		uint16_t iopb_offset;
	} __attribute__((packed));
}
