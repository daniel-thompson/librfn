/*
 * firstorder.c
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "librfn.h"
#include "libtt.h"

void tt_firstorder_init(tt_firstorder_t *fo, tt_context_t *ctx)
{
	tt_biquad_init(fo, ctx);
}

tt_firstorder_t *tt_firstorder_new(tt_context_t *ctx)
{
	return tt_biquad_new(ctx);
}

void tt_firstorder_delete(tt_firstorder_t *fo)
{
	tt_biquad_delete(fo);
}

ttspl_t tt_firstorder_step(tt_firstorder_t *fo, ttspl_t spl)
{
	return tt_biquad_step(fo, spl);
}

void tt_firstorder_process(tt_firstorder_t *fo, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	tt_biquad_process(fo, inbuf, outbuf);
}

void tt_firstorder_flush(tt_firstorder_t *fo)
{
	tt_biquad_flush(fo);
}

void tt_firstorder_lowpass(tt_firstorder_t *fo, int freq)
{
	fo->x[0] = TTINT(1);
	fo->x[1] = TTFLOAT(0.12);
	fo->x[2] = TTINT(0);
	fo->y[0] = TTFLOAT(exp((-M_PI * freq) / fo->ctx->sampling_frequency));
	fo->y[1] = TTINT(0);
}

void tt_firstorder_highpass(tt_firstorder_t *fo, int freq)
{
	fo->x[0] = TTINT(1);
	fo->x[1] = TTINT(-1);
	fo->x[2] = TTINT(0);
	fo->y[0] = TTFLOAT(exp((-M_PI * freq) / fo->ctx->sampling_frequency));
	fo->y[1] = TTINT(0);
}