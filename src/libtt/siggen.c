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

tt_siggen_t *tt_siggen_new()
{
	tt_siggen_t *sg = xmalloc(sizeof(tt_siggen_t));

	tt_siggen_setup(sg, 0, 0, TTINT(0), TT_SIGGEN_DC);

	return sg;
}

void tt_siggen_delete(tt_siggen_t *sg)
{
	free(sg);
}

ttspl_t tt_siggen_step(tt_siggen_t *sg)
{
	ttspl_t phase = sg->phase;

	sg->phase = TTADD(sg->phase, sg->step);
	if (sg->phase > sg->limit)
		sg->phase = TTSUB(sg->phase, sg->limit);

	return TTMAL(sg->fn(phase), sg->amplitude);
}

static ttspl_t process_step(void *sg)
{
	return tt_siggen_step(sg);
}

void tt_siggen_process(tt_siggen_t *sg, tt_sbuf_t *outbuf)
{
	tt_generic_output(process_step, sg, outbuf);
}

void tt_siggen_reset(tt_siggen_t *sg)
{
	sg->phase = TTINT(0);
}

void tt_siggen_setup(
		tt_siggen_t *sg,
		int sfreq, int gfreq, ttspl_t amplitude, tt_siggen_fn_t fn)
{
	ttspl_t limit = TTMINT(TTPI, 2);

	// TODO: For triangle wave it is easier to set limit to 1.0

	tlspl_t numerator = TTMUL(limit, TTINT(gfreq));
	if (sfreq) {
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
	default:
		assert(0);
		break;
	}
}