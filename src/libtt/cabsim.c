/*
 * cabsim.c
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

#include "librfn.h"
#include "libtt.h"

/*! Simple biquad based cabinet simulator.
 *
 * Starting point was based on a graph of the condor from runoffgroove.
 *
 * Five biquads:
 *     Partial notch filter at 400Hz (peaking EQ) (-16dB)
 *     High boosting shelf filter at 400Hz (6dB)
 *     High pass at 60Hz
 *     2 x low pass at 4000Hz
 *
 * fiview 48000 -i \
 *   PkBq/0.7/-16/400 x \
 *   HsBq/0.7/6/400 x \
 *   HpBq/0.7/60 x \
 *   LpBq/0.7/4000 x LpBq/0.7/4000
 *
 */
tt_cabsim_t *tt_cabsim_new()
{
	tt_cabsim_t *cabsim = xmalloc(sizeof(*cabsim));

	tt_biquad_init(&cabsim->notch);
	tt_biquad_init(&cabsim->shelf);
	tt_biquad_init(&cabsim->hpf);
	tt_biquad_init(&cabsim->lpf0);
	tt_biquad_init(&cabsim->lpf1);

	// TODO: Needs to move to setup function (and 48k needs to be parameterized)
	tt_biquad_peakingeq(&cabsim->notch, 48000, 400, -16, TTFLOAT(0.7));
	tt_biquad_highshelf(&cabsim->shelf, 48000, 400, 6, TTFLOAT(0.7));
	tt_biquad_highpass(&cabsim->hpf, 48000, 60, TTFLOAT(0.7));
	tt_biquad_lowpass(&cabsim->lpf0, 48000, 4000, TTFLOAT(0.7));
	tt_biquad_lowpass(&cabsim->lpf1, 48000, 4000, TTFLOAT(0.7));

	return cabsim;
}

void tt_cabsim_delete(tt_cabsim_t *cabsim)
{
	free(cabsim);
}

void tt_cabsim_process(tt_cabsim_t *cabsim, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	assert(inbuf->sz == outbuf->sz);

	tt_sbuf_t *tmpbuf = tt_sbuf_new(inbuf->sz);

	tt_biquad_process(&cabsim->notch, inbuf, outbuf);
	tt_biquad_process(&cabsim->shelf, outbuf, tmpbuf);
	tt_biquad_process(&cabsim->hpf, tmpbuf, outbuf);
	tt_biquad_process(&cabsim->lpf0, outbuf, tmpbuf);
	tt_biquad_process(&cabsim->lpf1, tmpbuf, outbuf);

	tt_sbuf_delete(tmpbuf);
}
