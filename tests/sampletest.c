/*
 * sampletest.c
 *
 * Part of libtt (the integer amplifier library)
 *
 * Copyright (C) 2012 Daniel Thompson <daniel@redfelineninja.org.uk> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <librfn.h>
#include <libtt.h>

int main()
{
	float minus6 = TTASFLOAT(TTLINEAR2DB(TTFLOAT(0.5)));
	printf("minus6 = %5.2fdB\n", minus6);
	assert(fuzzcmpf(-6, minus6, 1.01));

	float quarter = TTASFLOAT(TTDB2LINEAR(TTINT(-12)));
	printf("quarter = %9.6f\n", quarter);
	assert(fuzzcmpf(0.25, quarter, 1.01));


	return 0;
}
