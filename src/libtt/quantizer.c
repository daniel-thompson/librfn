/*
 * quantizer.c
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

#include <string.h>

#include "libtt.h"

void tt_quantizer_init(tt_quantizer_t *q, tt_context_t *ctx)
{
	memset(q, 0, sizeof(*q));
	q->ctx = ctx;
	q->shift = 0;
}

void tt_quantizer_finalize(tt_quantizer_t *q)
{
}

tt_generic_new(quantizer);
tt_generic_delete(quantizer);

void tt_quantizer_setup(tt_quantizer_t *q, unsigned int nbits)
{
	q->shift = 32 - nbits;
}

ttspl_t tt_quantizer_step(tt_quantizer_t *q, ttspl_t spl)
{
	int32_t ispl = TTASS32LE(spl);
	ispl >>= q->shift;
	ispl <<= q->shift;
	return TTS32LE(ispl);
}

tt_generic_process(quantizer);
