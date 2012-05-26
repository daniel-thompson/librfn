/*
 * sbuf.c
 *
 * Sample buffer operations for libtt (tintamp).
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

#include "libtt/sbuf.h"

tt_sbuf_t *tt_sbuf_new(unsigned sz)
{
	char *p = malloc(
		sizeof(tt_sbuf_t) +
		sizeof(ttspl_t) * (TT_SBUF_PREBUF + sz));

	if (p) {
		tt_sbuf_t *sbuf = (tt_sbuf_t *) p;
		sbuf->p = (ttspl_t *) (p + sizeof(tt_sbuf_t));
		sbuf->sz = sz;
		return sbuf;
	}

	return NULL;
}

void tt_sbuf_delete(tt_sbuf_t *sbuf)
{
	free(sbuf);
}

ttspl_t *tt_sbuf_at_rangechecked(tt_sbuf_t *sbuf, int at)
{
	assert(at < sbuf->sz);
	assert(at >= -TT_SBUF_PREBUF);

	return sbuf->p + at;
}

void tt_sbuf_to_s16le(tt_sbuf_t *sbuf, int16_t *p, unsigned len)
{
	assert(sbuf->sz == len);
	
	for (unsigned i=0; i<len; i++)
		p[i] = TT_SPL_TO_S16LE(sbuf->p[i]);
}

void tt_s16le_to_sbuf(int16_t *p, unsigned len, tt_sbuf_t *sbuf)
{
	assert(sbuf->sz == len);

	for (unsigned i=0; i<len; i++)
		sbuf->p[i] = TT_S16LE_TO_SPL(p[i]);
}

void tt_sbuf_to_float(tt_sbuf_t *sbuf, float *p, unsigned len)
{
	assert(sbuf->sz == len);
	
	for (unsigned i=0; i<len; i++)
		p[i] = TT_SPL_TO_FLOAT(sbuf->p[i]);
}

void tt_float_to_sbuf(float *p, unsigned len, tt_sbuf_t *sbuf)
{
	assert(sbuf->sz == len);

	for (unsigned i=0; i<len; i++)
		sbuf->p[i] = TT_FLOAT_TO_SPL(p[i]);
}
