/*
 * biquad.c
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


/*! \group biquad Biquad filters
 *
 * Like pretty much every other biquad library on the planet, this library is
 * basically a restatement of some [cookbook formulea][].
 *
 * [cookbook formulea]
 *     http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 *     "Cookbook formulae for audio EQ biquad filter coefficients"
 */

void tt_biquad_init(tt_biquad_t *bq, tt_context_t *ctx)
{
	memset(bq, 0, sizeof(*bq));
	bq->ctx = ctx;
}

tt_generic_new(biquad);
tt_generic_delete(biquad);

inline ttspl_t tt_biquad_step(tt_biquad_t *bq, ttspl_t spl)
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

tt_generic_process(biquad);

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

#ifdef BIQUAD_DEBUG
#define P(x) printf("%s = %12.6f\n", #x, (double) x)
	P(bd->A);
	P(bd->G);
	P(bd->w0);
	P(bd->sinw0);
	P(bd->cosw0);
	P(bd->alpha);

	P(bd->b[0]);
	P(bd->b[1]);
	P(bd->b[2]);
	P(bd->a[0]);
	P(bd->a[1]);
	P(bd->a[2]);

	P(bq->x[0]);
	P(bq->x[1]);
	P(bq->x[2]);
	P(bq->y[0]);
	P(bq->y[1]);
#endif
}

void tt_biquad_lowpass(tt_biquad_t *bq, int sfreq, int shfreq, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, shfreq, 0, q);

	ttspl_t common = TTSUB(TTINT(1), bd.cosw0);
	bd.b[0] = TTDINT(common, 2);
	bd.b[1] = common;
	bd.b[2] = bd.b[0];

	bd.a[0] = TTADD(TTINT(1), bd.alpha);
	bd.a[1] = TTMINT(bd.cosw0, -2);
	bd.a[2] = TTSUB(TTINT(1), bd.alpha);

	biquad_applydesign(bq, &bd);
}

void tt_biquad_highpass(tt_biquad_t *bq, int sfreq, int shfreq, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, shfreq, 0, q);

        ttspl_t common = TTADD(1, bd.cosw0);
        bd.b[0] = TTDINT(common, 2);
        bd.b[1] = TTNEGATE(common);
        bd.b[2] = bd.b[0];

	bd.a[0] = TTADD(TTINT(1), bd.alpha);
	bd.a[1] = TTMINT(bd.cosw0, -2);
	bd.a[2] = TTSUB(TTINT(1), bd.alpha);

	biquad_applydesign(bq, &bd);
}

void tt_biquad_bandpass(tt_biquad_t *bq, int sfreq, int cfreq, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, cfreq, 0, q);

        bd.b[0] = bd.alpha;
        bd.b[1] = 0;
        bd.b[2] = TTNEGATE(bd.b[0]);

	bd.a[0] = TTADD(TTINT(1), bd.alpha);
	bd.a[1] = TTMINT(bd.cosw0, -2);
	bd.a[2] = TTSUB(TTINT(1), bd.alpha);

	biquad_applydesign(bq, &bd);
}

void tt_biquad_bandstop(tt_biquad_t *bq, int sfreq, int cfreq, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, cfreq, 0, q);

        bd.b[0] = TTINT(1);
        bd.b[1] = TTMINT(bd.cosw0, -2);;
        bd.b[2] = TTINT(1);

	bd.a[0] = TTADD(TTINT(1), bd.alpha);
	bd.a[1] = TTMINT(bd.cosw0, -2);
	bd.a[2] = TTSUB(TTINT(1), bd.alpha);

	biquad_applydesign(bq, &bd);
}

void tt_biquad_allpass(tt_biquad_t *bq, int sfreq, int csfreq, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, csfreq, 0, q);

        bd.b[0] = TTSUB(TTINT(1), bd.alpha);
        bd.b[1] = TTMINT(bd.cosw0, -2);
        bd.b[2] = TTADD(TTINT(1), bd.alpha);

	bd.a[0] = bd.b[2];
	bd.a[1] = bd.b[1];
	bd.a[2] = bd.b[0];

	biquad_applydesign(bq, &bd);
}

void tt_biquad_peakingeq(tt_biquad_t *bq, int sfreq, int cfreq, int dbgain, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, cfreq, dbgain, q);

	ttspl_t ama = TTMAL(bd.alpha, bd.A);
        bd.b[0] = TTADD(TTINT(1), ama);
        bd.b[1] = TTMINT(bd.cosw0, -2);
        bd.b[2] = TTSUB(TTINT(1), ama);

        ttspl_t ada = TTRAD(bd.alpha, bd.A);
	bd.a[0] = TTADD(TTINT(1), ada);
	bd.a[1] = bd.b[1];
	bd.a[2] = TTSUB(TTINT(1), ada);

	biquad_applydesign(bq, &bd);
}

void tt_biquad_lowshelf(tt_biquad_t *bq, int sfreq, int shfreq, int dbgain, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, shfreq, dbgain, q);

	ttspl_t handi = TTMAL(TTMINT(TTSQRT(bd.A), 2), bd.alpha);
	ttspl_t apone = TTADDI(bd.A, 1);
	ttspl_t amone = TTSUBI(bd.A, 1);
	ttspl_t apcos = TTMAL(apone, bd.cosw0);
	ttspl_t amcos = TTMAL(amone, bd.cosw0);

	bd.b[0] = TTMAL(bd.A, TTADD(TTSUB(apone, amcos), handi));
	bd.b[1] = TTMAL(TTMINT(bd.A, 2), TTSUB(amone, apcos));
	bd.b[2] = TTMAL(bd.A, TTSUB(apone, TTADD(amcos, handi)));

	bd.a[0] = TTADD(apone, TTADD(amcos, handi));
	bd.a[1] = TTMINT(TTADD(amone, apcos), -2);
	bd.a[2] = TTADD(apone, TTSUB(amcos, handi));

#ifdef HAVE_FPU
	// double check design from basic equations
	float A = bd.A;
	float w0 = bd.w0;
	float alpha = bd.alpha;

        asserteq(bd.b[0],   A*( (A+1) - (A-1)*cosf(w0) + 2*sqrtf(A)*alpha ));
        asserteq(bd.b[1], 2*A*( (A-1) - (A+1)*cosf(w0)                    ));
        asserteq(bd.b[2],   A*( (A+1) - (A-1)*cosf(w0) - 2*sqrtf(A)*alpha ));
        asserteq(bd.a[0],       (A+1) + (A-1)*cosf(w0) + 2*sqrtf(A)*alpha  );
        asserteq(bd.a[1],  -2*( (A-1) + (A+1)*cosf(w0)                    ));
        asserteq(bd.a[2],       (A+1) + (A-1)*cosf(w0) - 2*sqrtf(A)*alpha  );
#endif

	biquad_applydesign(bq, &bd);
}

void tt_biquad_highshelf(tt_biquad_t *bq, int sfreq, int shfreq, int dbgain, ttspl_t q)
{
	biquad_design_t bd;

	biquad_predesign(&bd, sfreq, shfreq, dbgain, q);

	ttspl_t handi = TTMAL(TTMINT(TTSQRT(bd.A), 2), bd.alpha);
	ttspl_t apone = TTADDI(bd.A, 1);
	ttspl_t amone = TTSUBI(bd.A, 1);
	ttspl_t apcos = TTMAL(apone, bd.cosw0);
	ttspl_t amcos = TTMAL(amone, bd.cosw0);

	bd.b[0] = TTMAL(bd.A, TTADD(apone, TTADD(amcos, handi)));
	bd.b[1] = TTMAL(TTMINT(bd.A, -2), TTADD(amone, apcos));
	bd.b[2] = TTMAL(bd.A, TTADD(apone, TTSUB(amcos, handi)));

	bd.a[0] = TTSUB(TTADD(apone, handi), amcos);
	bd.a[1] = TTMINT(TTSUB(amone, apcos), 2);
	bd.a[2] = TTSUB(apone, TTADD(amcos, handi));

#ifdef HAVE_FPU
	float A = bd.A;
	float w0 = bd.w0;
	float alpha = bd.alpha;

	asserteq(bd.b[0],    A*( (A+1) + (A-1)*cosf(w0) + 2*sqrtf(A)*alpha ));
	asserteq(bd.b[1], -2*A*( (A-1) + (A+1)*cosf(w0)                    ));
	asserteq(bd.b[2],    A*( (A+1) + (A-1)*cosf(w0) - 2*sqrtf(A)*alpha ));
	asserteq(bd.a[0],        (A+1) - (A-1)*cosf(w0) + 2*sqrtf(A)*alpha  );
	asserteq(bd.a[1],    2*( (A-1) - (A+1)*cosf(w0)                    ));
	asserteq(bd.a[2],        (A+1) - (A-1)*cosf(w0) - 2*sqrtf(A)*alpha  );
#endif

	biquad_applydesign(bq, &bd);
}

char *tt_biquad_tostring(tt_biquad_t *bq)
{
	return strdup_printf(
		"y[n] = %9.6f x[n]  + %9.6f x[n-1]  + %9.6f x[n-2]  + %9.6f y[n-1]  + %9.6f y[n-2]\n",
		TTASFLOAT(bq->x[0]),
		TTASFLOAT(bq->x[1]),
		TTASFLOAT(bq->x[2]),
		TTASFLOAT(bq->y[0]),
		TTASFLOAT(bq->y[1]));
}
