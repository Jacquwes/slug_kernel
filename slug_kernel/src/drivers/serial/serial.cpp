/*
 * This file defines convenience functions for serial communication.
 */

#include <stdint.h>

#include "drivers/interrupts/pic.h"
#include "drivers/serial/serial.h"

namespace slug_kernel::drivers::serial
{
	using namespace interrupts::pic;

	void init_port(com_ports_t const& port)
	{
		// TODO: Make this more generic and less hardcoded

		// Disable interrupts
		outb(static_cast<uint16_t>(port) + static_cast<uint16_t>(com_registers_t::interrupt_enable), 0x00);

		// Set baud rate
		// Enable DLAB
		outb(static_cast<uint16_t>(port) + static_cast<uint16_t>(com_registers_t::line_control), 0xff);
		// Set divisor to 3 (38400 baud)
		outb(static_cast<uint16_t>(port) + static_cast<uint16_t>(com_registers_dlab_t::baud_divisor_rate_low), 0x03);
		outb(static_cast<uint16_t>(port) + static_cast<uint16_t>(com_registers_dlab_t::baud_divisor_rate_high), 0x00);

		// Disable DLAB and set data bits to 8
		outb(static_cast<uint16_t>(port) + static_cast<uint16_t>(com_registers_t::line_control), 0x03);

		// Set FIFO control register
		outb(static_cast<uint16_t>(port) + static_cast<uint16_t>(com_registers_t::interrupt_identification_fifo_control), 0xc7);

		// Set modem control register
		outb(static_cast<uint16_t>(port) + static_cast<uint16_t>(com_registers_t::modem_control), 0x0b);
	}

	void put_char(char const& c, com_ports_t const& port)
	{
		while (is_transmitting(port));

		outb(static_cast<uint16_t>(port), c);
	}

	void put_string(char const* str, com_ports_t const& port)
	{
		for (; *str; str++)
			put_char(*str, port);
	}

	bool is_transmitting(com_ports_t const& port)
	{
		return (inb(static_cast<uint16_t>(port) + static_cast<uint16_t>(com_registers_t::line_status)) & 0x20) == 0;
	}
}
