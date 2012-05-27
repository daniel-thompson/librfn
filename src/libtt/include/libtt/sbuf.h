/*
 * sbuf.h
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

#ifndef TT_SBUF_H_
#define TT_SBUF_H_

#include <stdint.h>

#include "sample.h"

#define TT_SBUF_PREBUF 4

/*! Sample buffer */
typedef struct {
	ttspl_t *p;
	unsigned sz;
} tt_sbuf_t;

tt_sbuf_t *tt_sbuf_new(unsigned sz);
void tt_sbuf_delete(tt_sbuf_t *sbuf);

ttspl_t *tt_sbuf_at_rangechecked(tt_sbuf_t *sbuf, int at);

#ifndef TT_SBUF_CHECKING
#define TTAT(sbuf, at) ((sbuf)->p[(at)])
#else
#define TTAT(sbuf, at) *(tt_sbuf_at_rangechecked(sbuf, at))
#endif

void tt_sbuf_to_s16le(tt_sbuf_t *sbuf, int16_t *p, unsigned len);
void tt_s16le_to_sbuf(int16_t *p, unsigned len, tt_sbuf_t *sbuf);
void tt_sbuf_to_float(tt_sbuf_t *sbuf, float *p, unsigned len);
void tt_float_to_sbuf(float *p, unsigned len, tt_sbuf_t *sbuf);

#endif // TT_SBUF_H_
