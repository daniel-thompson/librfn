/*
 * biquad.c
 *
 * Biquad operations for libtt (tintamp) 
 *
 * Copyright (C) 2012 Daniel Thompson <daniel@redfelineninja.org.uk> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "librfn.h"
#include "libtt.h"

tt_biquad_t *tt_biquad_new()
{
	return calloc(1, sizeof(tt_biquad_t));
}

void tt_biquad_free(tt_biquad_t *bq)
{
	free(bq);
}

ttspl_t tt_biquad_step(tt_biquad_t *bq, ttspl_t spl)
{
#ifdef HAVE_FPU
	tlspl_t acc = 0.0;
#else
	tlspl_t acc = bq->error_feedback;
#endif

	int nextzi = !bq->zi;

	TTMAC(acc, bq->x[0], spl);
	TTMAC(acc, bq->x[1], bq->zx[bq->zi]);
	TTMAC(acc, bq->x[2], bq->zx[nextzi]);
	TTMAC(acc, bq->y[0], bq->zy[bq->zi]);
	TTMAC(acc, bq->y[1], bq->zy[nextzi]);

	ttspl_t y = TLLOWER(acc);
	
	bq->zx[nextzi] = spl;
	bq->zy[nextzi] = y;
	bq->zi = nextzi;
#ifndef HAVE_FPU
	bq->error_feedback = TLERROR(acc);
#endif

	return y;
}

void tt_biquad_process(tt_biquad_t *bq, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	tt_generic_mono_process(tt_biquad_step, bq, inbuf, outbuf);
}


void tt_biquad_flush(tt_biquad_t *bq)
{
	memset(bq->zx, 0, sizeof(bq->zx));
	memset(bq->zy, 0, sizeof(bq->zy));
}

typedef struct biquad_design {
	ttspl_t A;
	ttspl_t G;
	ttspl_t w0;
	ttspl_t cosw0;
	ttspl_t sinw0;

	ttspl_t alpha;

	ttspl_t b[3];
	ttspl_t a[3];

	ttspl_t x[3];
	ttspl_t y[2];
} biquad_design_t;

/*!
 * Pre-compute a few intermediate variables common to all filters.
 */
static void biquad_predesign(biquad_design_t *bd, int sfreq, int freq, int dbgain, ttspl_t q)
{
	ttspl_t gain = TTINT(dbgain);
	ttspl_t f0 = TTINT(freq);

	gain = TTDINT(gain, 20);
	bd->A = TTSQRT(TTPOW(TTINT(10), gain));

	gain = TTDINT(gain, 2); // gain = dbgain / 40
	bd->G = TTPOW(TTINT(10), gain);

	tlspl_t numerator = TTMUL(TTMINT(TTPI, 2), f0);
	bd->w0 = TLDIV(numerator, TTINT(sfreq));

	bd->cosw0 = TTCOS(bd->w0);
	bd->sinw0 = TTSIN(bd->w0);

	bd->alpha = TTRAD(bd->sinw0, TTMINT(q, 2));
}

static void biquad_applydesign(tt_biquad_t *bq, biquad_design_t *bd)
{
	bq->x[0] = TTRAD(bd->b[0], bd->a[0]);
	bq->x[1] = TTRAD(bd->b[1], bd->a[0]);
	bq->x[2] = TTRAD(bd->b[2], bd->a[0]);
	bq->y[0] = TTNEGATE(TTRAD(bd->a[1], bd->a[0]));
	bq->y[1] = TTNEGATE(TTRAD(bd->a[2], bd->a[0]));
}

void tt_biquad_lowpass(tt_biquad_t *bq, int sfreq, int shfreq, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, shfreq, 0, q);

	bd.b[1] = TTSUB(TTINT(1), bd.cosw0);
	bd.b[0] = TTRAD(bd.b[1], TTINT(2));
	bd.b[2] = bd.b[0];

	bd.a[0] = TTADD(TTINT(1), bd.alpha);
	bd.a[1] = TTMAL(TTINT(-2), bd.cosw0);
	bd.a[2] = TTSUB(TTINT(1), bd.alpha);

	biquad_applydesign(bq, &bd);
}

void tt_biquad_highpass(tt_biquad_t *bq, int sfreq, int shfreq, ttspl_t q)
{
	assert(0);
}

void tt_biquad_bandpass(tt_biquad_t *bq, int sfreq, int cfreq, ttspl_t q)
{
	assert(0);
}

void tt_biquad_bandstop(tt_biquad_t *bq, int sfreq, int cfreq, ttspl_t q)
{
	assert(0);
}

void tt_biquad_allpass(tt_biquad_t *bq, int csfreq, ttspl_t q)
{
	assert(0);
}

void tt_biquad_peakingeq(tt_biquad_t *bq, int sfreq, int cfreq, int dbgain, ttspl_t q)
{
	assert(0);
}

void tt_biquad_lowshelf(tt_biquad_t *bq, int sfreq, int shfreq, int dbgain, ttspl_t q)
{
	assert(0);
}

void tt_biquad_highshelf(tt_biquad_t *bq, int sfreq, int shfreq, int dbgain, ttspl_t q)
{
	assert(0);
}

char *tt_biquad_tostring(tt_biquad_t *bq)
{
	return strdup_printf(
		"y[n] = %f x[n]  +  %f x[n-1]  +  %f x[n-2]  +  %f y[n-1]  +  %f y[n-2]\n",
		TTASFLOAT(bq->x[0]),
		TTASFLOAT(bq->x[1]),
		TTASFLOAT(bq->x[2]),
		TTASFLOAT(bq->y[0]),
		TTASFLOAT(bq->y[1]));
}
