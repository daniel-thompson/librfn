/*
 * siggentest.c
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
	tt_siggen_t *sg = tt_siggen_new();
	assert(sg);
	tt_siggen_setup(sg, 48000, 440, TTINT(2), TT_SIGGEN_SIN);

	tt_sbuf_t *sbuf = tt_sbuf_new(48000);
	assert(sbuf);
	tt_siggen_process(sg, sbuf);

	// TEST: peak tracking
	ttspl_t peak = tt_analyse_peak(sbuf);
	printf("peak %9.6f (%5.2fdB)\n", TTASFLOAT(peak), TTASFLOAT(TTLINEAR2DB(peak)));
	assert(fuzzcmpf(TTASFLOAT(peak), 2, 1.01));


	// TEST: average value tracking
	ttspl_t avg = tt_analyse_rectify(sbuf);
	printf("rect %9.6f (%5.2fdB)\n", TTASFLOAT(avg), TTASFLOAT(TTLINEAR2DB(avg)));
	assert(fuzzcmpf(TTASFLOAT(avg), 1.27, 1.01));

	tt_sbuf_delete(sbuf);
	tt_siggen_delete(sg);

	return 0;
}
