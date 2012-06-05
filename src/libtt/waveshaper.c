/*
 * waveshaper.c
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
#include <string.h>

#include "libtt.h"

void tt_waveshaper_init(tt_waveshaper_t *ws)
{
	memset(ws, 0, sizeof(*ws));
}

void tt_waveshaper_setup(tt_waveshaper_t *ws,
		ttspl_t min, ttspl_t max,
		const ttspl_t *shape, unsigned int shapelen)
{
	assert(min < max);
	assert(shape); // won't be dereferenced in this function
	assert(shapelen > 2);
	assert(shapelen < TTSPL_MAX); // maths will overflow otherwise

	ws->min = min;
	ws->limit = TTSUB(max, min);
	ws->limit_as_int = TTASINT(ws->limit);
	ws->limit_is_int = (ws->limit == TTINT(ws->limit_as_int));

	ws->shape.p = (ttspl_t *) shape; // cast is to removes constness
	ws->shape.sz = shapelen;
}

inline ttspl_t tt_waveshaper_step(tt_waveshaper_t *ws, ttspl_t in)
{
	if (in <= ws->min)
		return TTAT(&ws->shape, 0);

	in = TTSUB(in, ws->min);
	if (in >= ws->limit)
		return TTAT(&ws->shape, ws->shape.sz-1);

	// try to avoid 64-bit divide...
	if (ws->limit_is_int)
		in = TTDINT(in, ws->limit_as_int);
	else
		in = TTRAD(in, ws->limit);

	// in must now be in the range 0..1)	
	assert(TTINT(0) <= in && TTINT(in <= 1));

	ttspl_t index = TTMINT(in, ws->shape.sz-1);
	unsigned int i = TTINT(index);
	ttspl_t aspl = TTAT(&ws->shape, i+1);
	ttspl_t aweight = TTFRACTION(index);
	ttspl_t bspl = TTAT(&ws->shape, i);
	ttspl_t bweight = TTSUB(TTINT(1), aweight);

	tlspl_t acc = TTMUL(aspl, aweight);
	TTMAC(acc, bspl, bweight);
	
	return TLLOWER(acc);
}
tt_generic_process(waveshaper);

