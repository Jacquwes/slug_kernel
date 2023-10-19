#pragma once

/*
 * This file defines convenience functions for serial communication.
 */

#include <stdint.h>

namespace slug_kernel::drivers::serial
{
	// COM ports for serial communication
	enum class com_ports_t : uint16_t
	{
		com_1 = 0x3F8,
		com_2 = 0x2F8,
		com_3 = 0x3E8,
		com_4 = 0x2E8,
		com_5 = 0x5F8,
		com_6 = 0x4F8,
		com_7 = 0x5E8,
		com_8 = 0x4E8,
	};

	// COM port registers
	// The most significant bit of line_control is the divisor latch access bit
	enum class com_registers_t : uint16_t
	{
		data = 0,
		interrupt_enable = 1,
		interrupt_identification_fifo_control = 2,
		line_control = 3,
		modem_control = 4,
		line_status = 5,
		modem_status = 6,
		scratch = 7,
	};

	// COM port registers with divisor latch access bit set
	enum class com_registers_dlab_t : uint16_t
	{
		baud_divisor_rate_low = 0,
		baud_divisor_rate_high = 1,
	};

	void init_port(com_ports_t const& port = com_ports_t::com_1);

	void put_char(char const& c, com_ports_t const& port = com_ports_t::com_1);
	void put_string(char const* str, com_ports_t const& port = com_ports_t::com_1);

	bool is_transmitting(com_ports_t const& port = com_ports_t::com_1);
}