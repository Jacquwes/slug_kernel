#pragma once

/*
* This file defines the driver to interact with the PIC.
*/

#include <stdint.h>

namespace slug_kernel::drivers::interrupts::pic

{
	enum class pic_ports_t : uint16_t
	{
		master_command = 0x20,
		master_data = 0x21,
		slave_command = 0xA0,
		slave_data = 0xA1,
	};

	// PIC initialization control words
	enum class icw1_t : uint8_t
	{
		icw4_needed = 0x01,
		single = 0x02,
		interval_4 = 0x04,
		level_triggered = 0x08,
		init = 0x10
	};

	// PIC vector offsets	
	enum class icw2_t : uint8_t
	{
		master_offset = static_cast<uint8_t>(pic_ports_t::master_command),
		slave_offset = static_cast<uint8_t>(pic_ports_t::slave_command),
	};

	// How the PIC is connected to the CPU
	enum class icw3_t : uint8_t
	{
		master_slave_line = 0x04,
		slave_id = 0x02,
	};

	// Information about the PIC
	enum class icw4_t : uint8_t
	{
		_8086 = 0x01,
		auto_eoi = 0x02,
		buffered_mode_slave = 0x08,
		buffered_mode_master = 0x0c,
		special_fully_nested = 0x10,
	};

	void init();

	void outb(uint16_t const& port, uint8_t const& data);
	uint8_t inb(uint16_t const& port);
}
