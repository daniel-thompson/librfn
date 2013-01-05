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

#include <libtt.h>

int main()
{
	tt_context_t *ctx = tt_context_new();
	tt_siggen_t *sg = tt_siggen_new(ctx);
	assert(sg);

	// TEST: muted by default
	assert(TTINT(0) == tt_siggen_step(sg));
	assert(TTINT(0) == tt_siggen_step(sg));

	// CFG: 2400Hz, amplitude of 2
	tt_siggen_setup(sg, 2400, TTINT(2), TT_SIGGEN_SIN);

	// TEST: phase is 0 at reset
	tt_siggen_reset(sg);
	assert(TTINT(0) == tt_siggen_step(sg));

	// TEST: whole phase at fs/20 with no quantization errors
	tt_siggen_reset(sg);
	ttspl_t phase[21];
	for (int i=0; i<21; i++)
		phase[i] = tt_siggen_step(sg);
	assert(TTINT(0) == phase[0]);
	assert(TTINT(2) == phase[5]);
	//assert(TTINT(0) == phase[10]); // needs rounding before check with FPU
	assert(TTINT(-2) == phase[15]);
	//assert(TTINT(0) == phase[20]); // needs rounding before check with FPU

	// TEST: whole phase via _process()
	tt_siggen_reset(sg);
	tt_sbuf_t *sbuf = tt_sbuf_new(42);
	assert(sbuf);
	tt_siggen_process(sg, sbuf);
	for (int i=0; i<21; i++)
		assert(phase[i] == TTAT(sbuf, i));

	// TIDY
	tt_siggen_delete(sg);
	tt_sbuf_delete(sbuf);
	tt_context_delete(ctx);

	return 0;
}
