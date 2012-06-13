/*
 * siggen.c
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
#include <stdlib.h>
#include <string.h>

#include "librfn.h"
#include "libtt.h"

static ttspl_t generate_constant(ttspl_t phase)
{
	return TTINT(1);
}

static ttspl_t generate_sin(ttspl_t phase)
{
	return TTSIN(phase);
}

static ttspl_t generate_white_noise(ttspl_t phase)
{
	return TTFLOAT((float) (2 * drand48()) - 1);
}

void tt_siggen_init(tt_siggen_t *sg, tt_context_t *ctx)
{
	memset(sg, 0, sizeof(*sg));
	sg->ctx = ctx;
	tt_siggen_setup(sg, 0, TTINT(0), TT_SIGGEN_DC);
}

void tt_siggen_finalize(tt_siggen_t *sg)
{
}

tt_generic_new(siggen);
tt_generic_delete(siggen);

inline ttspl_t tt_siggen_step(tt_siggen_t *sg)
{
	ttspl_t phase = sg->phase;

	sg->phase = TTADD(sg->phase, sg->step);
	if (sg->phase > sg->limit)
		sg->phase = TTSUB(sg->phase, sg->limit);

	return TTMAL(sg->fn(phase), sg->amplitude);
}

tt_generic_output(siggen);

void tt_siggen_reset(tt_siggen_t *sg)
{
	sg->phase = TTINT(0);
}

void tt_siggen_setup(
		tt_siggen_t *sg,
		int gfreq, ttspl_t amplitude, tt_siggen_fn_t fn)
{
	int sfreq = sg->ctx->sampling_frequency;
	ttspl_t limit = TTMINT(TTPI, 2);

	// TODO: For triangle wave it is easier to set limit to 1.0

	tlspl_t numerator = TTMUL(limit, TTINT(gfreq));
	if (gfreq) {
		sg->step = TLDIV(numerator, TTINT(sfreq));
		sg->limit = limit;
	} else {
		sg->step = 0;
		sg->limit = TTADD(amplitude, TTINT(1));
	}

	sg->amplitude = amplitude;

	switch(fn) {
	case TT_SIGGEN_DC:
		sg->fn = generate_constant;
		break;
	case TT_SIGGEN_SIN:
		sg->fn = generate_sin;
		break;
	case TT_SIGGEN_WHITE_NOISE:
		sg->fn = generate_white_noise;
		break;
	default:
		assert(0);
		break;
	}
}
