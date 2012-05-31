/*
 * waveshaper.h
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

#ifndef TT_WAVESHAPER_H_
#define TT_WAVESHAPER_H_

#include <stdbool.h>

#include "sample.h"
#include "sbuf.h"

typedef struct {
	ttspl_t min;
	// no max since limit and friends can be used for this
	ttspl_t limit;
	unsigned int limit_as_int;
	bool limit_is_int;

	tt_sbuf_t shape;
} tt_waveshaper_t;

void tt_waveshaper_init(tt_waveshaper_t *ws);
void tt_waveshaper_setup(tt_waveshaper_t *ws,
		ttspl_t min, ttspl_t max,
		const ttspl_t *shape, unsigned int shapelen);

ttspl_t tt_waveshaper_step(tt_waveshaper_t *ws, ttspl_t in);
void tt_waveshaper_process(tt_waveshaper_t *ws, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_WAVESHAPER_H_
