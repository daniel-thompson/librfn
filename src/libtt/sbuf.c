/*
 * sbuf.c
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
#include <stdbool.h>
#include <stdlib.h>

#include "librfn.h"
#include "libtt.h"

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
		p[i] = TTASS16LE(sbuf->p[i]);
}

void tt_s16le_to_sbuf(int16_t *p, unsigned len, tt_sbuf_t *sbuf)
{
	assert(sbuf->sz == len);

	for (unsigned i=0; i<len; i++)
		sbuf->p[i] = TTS16LE(p[i]);
}

void tt_sbuf_to_float(tt_sbuf_t *sbuf, float *p, unsigned len)
{
	assert(sbuf->sz == len);
	
	for (unsigned i=0; i<len; i++)
		p[i] = TTASFLOAT(sbuf->p[i]);
}

void tt_float_to_sbuf(float *p, unsigned len, tt_sbuf_t *sbuf)
{
	assert(sbuf->sz == len);

	for (unsigned i=0; i<len; i++)
		sbuf->p[i] = TTFLOAT(p[i]);
}

char *tt_sbuf_tostring(tt_sbuf_t *sbuf, unsigned int lines)
{
	char *p = xstrdup_printf("");

	if ((sbuf->sz / 7) < lines)
		lines = sbuf->sz / 7;

	for (unsigned int i=0; i<lines; i++) {
		bool last_line = i && (i == lines - 1);
		bool penultimate_line = (i == lines - 2);
		int offset = (last_line ? sbuf->sz - 7 : i*7);

		char *q = xstrdup_printf(
				"%s%s%9.3f %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f%s",
				p,
				(last_line ? " ... " : "     "),
				TTASFLOAT(TTAT(sbuf, offset + 0)),
				TTASFLOAT(TTAT(sbuf, offset + 1)),
				TTASFLOAT(TTAT(sbuf, offset + 2)),
				TTASFLOAT(TTAT(sbuf, offset + 3)),
				TTASFLOAT(TTAT(sbuf, offset + 4)),
				TTASFLOAT(TTAT(sbuf, offset + 5)),
				TTASFLOAT(TTAT(sbuf, offset + 6)),
				(penultimate_line ? " ...\n" :
				 last_line        ? "" :
						    "\n"));
		free(p);
		p = q;
	}

	return p;
}
