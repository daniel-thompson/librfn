/*
 * drummachinetest.c
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

int main(int argc, char **argv)
{
	int res;

	// INIT
	tt_context_t *ctx = tt_context_new();

	tt_sbuf_t *outbuf = tt_sbuf_new(ctx->grain_size);
	tt_wavwriter_t *ww = tt_wavwriter_new(ctx);
	tt_drummachine_t *dm = tt_drummachine_new(ctx);

	res = tt_wavwriter_setup(ww, "drummachinetest.wav", RF_WAVHEADER_S32LE);
	assert(0 == res);

	tt_drummachine_set_control(dm, TT_DRUMMACHINE_CONTROL_PATTERN, TTINT(2));
	tt_drummachine_set_control(dm, TT_DRUMMACHINE_CONTROL_BPM, TTINT(120));

	int i;
	for (i=0; i<200; i++) {
		tt_drummachine_process(dm, outbuf);
		tt_wavwriter_process(ww, outbuf);
	}
	printf("Completed after %d loops\n", i);

	// TIDY
	tt_drummachine_delete(dm);
	tt_wavwriter_delete(ww);
	tt_sbuf_delete(outbuf);
	tt_context_delete(ctx);

	return 0;
}
