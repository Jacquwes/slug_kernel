/*
* This file implements functions for character array manipulation.
*/

#include "stdlib.h"
#include "string.h"

void* memmove(void* dest, void const* src, size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		reinterpret_cast<char*>(dest)[i] = reinterpret_cast<char const*>(src)[i];
	}

	return dest;
}
