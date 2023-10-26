/*
* This file implements the «print» components of stdio.h.
*/

#include <drivers/serial/serial.h>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

using namespace slug_kernel::drivers::serial;

static void itoa(char* string, size_t size, uint64_t n, int base)
{
	size_t i = 0;
	do
	{
		char digit = n % base;
		string[size - i - 2] = (digit < 10) ? (digit + '0') : (digit + 'a' - 10);
		i++;
	} while (n /= base);

	string[size - 1] = 0;
	memmove(string, string + size - i - 1, i + 1);
}

int printf(char const* format, ...)
{
	__builtin_va_list args{};
	__builtin_va_start(args, format);
	int r = vprintf(format, args);
	__builtin_va_end(args);

	return r;
}

int vprintf(char const* format, __builtin_va_list args)
{
	int written = 0;

	for (char const* c = format; *c; c++)
	{
		if (*c != '%')
		{
			put_char(*c);
			written++;
			continue;
		}

		switch (*(++c))
		{
		case 'c':
		{
			char glyph = static_cast<char>(__builtin_va_arg(args, int));

			put_char(glyph);
			written++;

			break;
		}
		case 'd':
		{
			int value = __builtin_va_arg(args, int);

			if (value < 0)
			{
				put_char('-');
				value = -value;
				written++;
			}

			if (!value)
			{
				put_char('0');
				written++;
				break;
			}

			size_t size = 36;

			char string[size];
			itoa(string, size, value, 10);

			put_string(string);
			written += strlen(string);

			break;
		}
		case 's':
		{
			char* str = reinterpret_cast<char*>(__builtin_va_arg(args, int));

			put_string(str);
			written += strlen(str);

			break;
		}
		default:
			break;
		}
	}

	return written;
}
