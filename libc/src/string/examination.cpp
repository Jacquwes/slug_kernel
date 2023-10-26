/*
* This file implements functions for string examination.
*/

#include "stdlib.h"
#include "string.h"

size_t strlen(char const* str)
{
	size_t length = 0;

	for (; *str; str++, length++);

	return length;
}
