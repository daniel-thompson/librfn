/*
 * waveshapertest.c
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
#include <string.h>

#include <librfn.h>
#include <libtt.h>

ttspl_t line[] = { TTINT(-1), TTINT(0), TTINT(1) };

ttspl_t absolute[] = { TTINT(1), TTINT(0), TTINT(1) };

bool compare_shape(tt_waveshaper_t *ws, float in, float expected)
{
	ttspl_t out = tt_waveshaper_step(ws, TTFLOAT(in));

	bool ok = fuzzcmpf(out, expected, 1.001f);

	printf("    %7.3f -> waveshaper -> %7.3f (expected %7.3f)  [ %s ]\n",
			in, TTASFLOAT(out), expected, ok ? " OK " : "FAIL");

	return ok;
}

int main()
{
	tt_context_t *ctx = tt_context_new();
	tt_waveshaper_t *ws = xmalloc(sizeof(*ws));
	assert(ws);

	tt_waveshaper_init(ws);

	printf("Direct mapping between -1 and 1:\n");
	tt_waveshaper_setup(ws, TTINT(-1), TTINT(1), line, lengthof(line));
	assert(compare_shape(ws, -2.00, -1.00));
	assert(compare_shape(ws, -1.01, -1.00));
	assert(compare_shape(ws, -1.00, -1.00));
	assert(compare_shape(ws, -0.75, -0.75));
	assert(compare_shape(ws, -0.50, -0.50));
	assert(compare_shape(ws, -0.25, -0.25));
	assert(compare_shape(ws,  0.00,  0.00));
	assert(compare_shape(ws,  0.25,  0.25));
	assert(compare_shape(ws,  0.50,  0.50));
	assert(compare_shape(ws,  0.75,  0.75));
	assert(compare_shape(ws,  1.00,  1.00));
	assert(compare_shape(ws,  1.01,  1.00));
	assert(compare_shape(ws,  2.00,  1.00));

	printf("Divide by 10:\n");
	tt_waveshaper_setup(ws, TTINT(-10), TTINT(10), line, lengthof(line));
	assert(compare_shape(ws, -5, -0.5));
	assert(compare_shape(ws, 3, 0.3));

	printf("Multiply by 10:\n");
	tt_waveshaper_setup(ws, TTFLOAT(-0.1), TTFLOAT(0.1), line, lengthof(line));
	assert(compare_shape(ws, -5, -1));
	assert(compare_shape(ws, 3, 1));
	assert(compare_shape(ws, 0.01, 0.1));
	assert(compare_shape(ws, 0.03, 0.3));

	printf("Absolute value (between -1 and 1):\n");
	tt_waveshaper_setup(ws, TTINT(-1), TTINT(1), absolute, lengthof(line));
	assert(compare_shape(ws, -2.00,  1.00));
	assert(compare_shape(ws, -1.01,  1.00));
	assert(compare_shape(ws, -1.00,  1.00));
	assert(compare_shape(ws, -0.75,  0.75));
	assert(compare_shape(ws, -0.50,  0.50));
	assert(compare_shape(ws, -0.25,  0.25));
	assert(compare_shape(ws,  0.00,  0.00));
	assert(compare_shape(ws,  0.25,  0.25));
	assert(compare_shape(ws,  0.50,  0.50));
	assert(compare_shape(ws,  0.75,  0.75));
	assert(compare_shape(ws,  1.00,  1.00));
	assert(compare_shape(ws,  1.01,  1.00));
	assert(compare_shape(ws,  2.00,  1.00));

	printf("Sample buffer test:\n");

	// create in input buffer and fill it with samples
	tt_sbuf_t *inbuf = tt_sbuf_new(512);
	assert(inbuf);
	tt_siggen_t *sg = tt_siggen_new(ctx);
	// choice of frequency is hack to maintain numeric stablity when
	// working with FPU (doesn't come to zero)
	tt_siggen_setup(sg, 2390, TTINT(1), TT_SIGGEN_SIN);
	tt_siggen_process(sg, inbuf);

	tt_sbuf_t *outbuf = tt_sbuf_new(512);
	assert(outbuf);

	tt_waveshaper_setup(ws, TTINT(-1), TTINT(1), line, lengthof(line));
	tt_waveshaper_process(ws, inbuf, outbuf);

	char *s = tt_sbuf_tostring(inbuf, 512); // will automatically shrink
	char *t = tt_sbuf_tostring(outbuf, 512);
	if (0 != strcmp(s, t)) {
		printf("Inbuf:\n%s\n", s);
		printf("Outbuf:\n%s\n", t);
		assert(0);
	}
	free(s);
	free(t);

	for (unsigned int i=0; i<inbuf->sz; i++) {
		float in = TTASFLOAT(TTAT(inbuf, i));
		float out = TTASFLOAT(TTAT(outbuf, i));

		bool ok = fuzzcmpf(in, out, 1.001f);
#if 0
		printf("    %7.3f -> waveshaper -> %7.3f (expected %7.3f)  [ %s ]\n",
				in, out, in, ok ? " OK " : "FAIL");
#endif
		assert(ok);
	}

	tt_siggen_delete(sg);
	tt_sbuf_delete(inbuf);
	tt_sbuf_delete(outbuf);
	free(ws);
	tt_context_delete(ctx);

	return 0;
}
