/*
 * firstorder.h
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

#ifndef TT_FIRSTORDER_H_
#define TT_FIRSTORDER_H_

#include <stdbool.h>

#include "biquad.h"
#include "sbuf.h"
#include "util.h"

typedef tt_biquad_t tt_firstorder_t;

void tt_firstorder_init(tt_firstorder_t *fo, tt_context_t *ctx);
static inline void tt_firstorder_finalize(tt_firstorder_t *fo) {}
tt_firstorder_t *tt_firstorder_new(tt_context_t *ctx);
void tt_firstorder_delete(tt_firstorder_t *fo);

ttspl_t tt_firstorder_step(tt_firstorder_t *fo, ttspl_t spl);
void tt_firstorder_process(tt_firstorder_t *fo, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);
void tt_firstorder_flush(tt_firstorder_t *fo);

void tt_firstorder_lowpass(tt_firstorder_t *fo, int freq);
void tt_firstorder_highpass(tt_firstorder_t *fo, int freq);

#endif // TT_FIRSTORDER_H_
