/*
 * delaytest.c
 *
 * Part of libtt (the integer amplifier library)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
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
	// INIT
	tt_context_t *ctx = tt_context_new();
	tt_sbuf_t *inbuf = tt_sbuf_new(ctx->grain_size);
	tt_sbuf_t *outbuf = tt_sbuf_new(ctx->grain_size);
	tt_siggen_t *sg = tt_siggen_new(ctx);
	tt_delay_t *delay = tt_delay_new(ctx, ctx->grain_size / 4);

	tt_siggen_setup(sg, 400, 1.0, TT_SIGGEN_WHITE_NOISE);
	tt_delay_setup(delay);

	// test initial state
	tt_siggen_process(sg, inbuf);
	tt_delay_process(delay, inbuf, outbuf);
	for (int i=0; i<ctx->grain_size/4; i++)
		verify(0 == TTAT(outbuf, i));
	for (int i=ctx->grain_size/4; i<ctx->grain_size; i++)
		verify(TTAT(inbuf, i - ctx->grain_size/4) ==
				TTAT(outbuf, i));

	// continuation
	tt_siggen_process(sg, inbuf);
	tt_delay_process(delay, inbuf, outbuf);
	for (int i=ctx->grain_size/4; i<ctx->grain_size; i++)
		verify(TTAT(inbuf, i - ctx->grain_size/4) ==
				TTAT(outbuf, i));

	// half the delay size
	tt_delay_set_control(delay, TT_DELAY_CONTROL_NUM_SAMPLES, TTENCODEINT(ctx->grain_size/8));
	tt_siggen_process(sg, inbuf);
	tt_delay_process(delay, inbuf, outbuf);
	for (int i=0; i<ctx->grain_size/8; i++)
		verify(0 == TTAT(outbuf, i));
	for (int i=ctx->grain_size/8; i<ctx->grain_size; i++)
		verify(TTAT(inbuf, i - ctx->grain_size/8) ==
				TTAT(outbuf, i));

	// set an overlarge delay size
	tt_delay_set_control(delay, TT_DELAY_CONTROL_NUM_SAMPLES, TTENCODEINT(ctx->grain_size));
	ttspl_t num_samples = tt_delay_get_control(delay, TT_DELAY_CONTROL_NUM_SAMPLES);
	verify(TTDECODEINT(num_samples) == ctx->grain_size/4);

	// TIDY
	tt_delay_delete(delay);
	tt_siggen_delete(sg);
	tt_sbuf_delete(outbuf);
	tt_sbuf_delete(inbuf);
	tt_context_delete(ctx);

	return 0;
}
