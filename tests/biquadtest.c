/*
 * biquadtest.c
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

#include <libtt.h>

int main()
{
	char *s;

	tt_biquad_t *bq = tt_biquad_new();
	assert(bq);

	s = tt_biquad_tostring(bq);
	assert(s);
	printf("%s\n", s);
	free(s);

	tt_biquad_lowpass(bq, 48000, 4000, TTFLOAT(0.7));
	s = tt_biquad_tostring(bq);
	assert(s);
	printf("%s\n", s);
	free(s);

	return 0;
}
