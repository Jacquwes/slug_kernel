/*
 * This file defines the driver to interact with the PIC.
 */

#include <stdint.h>

#include "drivers/interrupts/pic.h"

namespace slug_kernel::drivers::interrupts::pic
{
	void init()
	{
		// Start initialization sequence
		outb(static_cast<uint16_t>(pic_ports_t::master_command),
			static_cast<uint8_t>(icw1_t::init) | static_cast<uint8_t>(icw1_t::icw4_needed));
		outb(static_cast<uint16_t>(pic_ports_t::slave_command), static_cast<uint8_t>(icw1_t::init) |
			static_cast<uint8_t>(icw1_t::icw4_needed));

		// Set the vector offsets
		outb(static_cast<uint16_t>(pic_ports_t::master_command), static_cast<uint8_t>(icw2_t::master_offset));
		outb(static_cast<uint16_t>(pic_ports_t::slave_command), static_cast<uint8_t>(icw2_t::slave_offset));

		// Connect master PIC to slave PIC
		outb(static_cast<uint16_t>(pic_ports_t::master_command), static_cast<uint8_t>(icw3_t::master_slave_line));
		outb(static_cast<uint16_t>(pic_ports_t::slave_command), static_cast<uint8_t>(icw3_t::slave_id));

		// Set the mode
		outb(static_cast<uint16_t>(pic_ports_t::master_command), static_cast<uint8_t>(icw4_t::_8086));
		outb(static_cast<uint16_t>(pic_ports_t::slave_command), static_cast<uint8_t>(icw4_t::_8086));
	}

	void outb(uint16_t const& port, uint8_t const& data)
	{
		asm volatile (
			"outb %0, %1"
			: /* no output */
			: "a"(data), "Nd"(port)
		);
	}

	uint8_t inb(uint16_t const& port)
	{
		uint8_t data{};
		asm volatile (
			"inb %1, %0"
			: "=a"(data)
			: "Nd"(port)
		);
		return data;
	}
}
