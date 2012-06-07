/*
 * cabsimtest.c
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
	// INIT
	tt_context_t *ctx = tt_context_new();
	tt_sbuf_t *inbuf = tt_sbuf_new(ctx->grain_size);
	tt_sbuf_t *outbuf = tt_sbuf_new(ctx->grain_size);
	tt_siggen_t *sg = tt_siggen_new(ctx);
	tt_cabsim_t *cabsim = tt_cabsim_new(ctx);

	tt_siggen_setup(sg, 400, 1.570793, TT_SIGGEN_SIN);

	tt_siggen_process(sg, inbuf);
	tt_cabsim_process(cabsim, inbuf, outbuf);

	assert(TTAT(inbuf, 0) == TTINT(0));
	assert(TTAT(outbuf, 0) == TTINT(0));
	assert(TTAT(inbuf, ctx->grain_size-1) != TTINT(0));
	assert(TTAT(outbuf, ctx->grain_size-1) != TTINT(0));

	// TIDY
	tt_cabsim_delete(cabsim);
	tt_siggen_delete(sg);
	tt_sbuf_delete(outbuf);
	tt_sbuf_delete(inbuf);
	tt_context_delete(ctx);

	return 0;
}
