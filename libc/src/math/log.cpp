/*
* This file implements the logarithmic components of math.h.
*/

#include "math.h"

float log2f(float x)
{
	float val;
	asm volatile(
		"fld1;"
		"fld %[x];"
		"fyl2x;"
		"fstp %[val];"
		: [val] "=m" (val)
		: [x] "m" (x)
		);
	return val;
}
