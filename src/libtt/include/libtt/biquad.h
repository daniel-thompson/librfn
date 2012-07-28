/*
 * biquad.h
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

#ifndef TT_BIQUAD_H_
#define TT_BIQUAD_H_

#include <stdbool.h>

#include "sbuf.h"
#include "util.h"

typedef struct {
	tt_context_t *ctx;

	ttspl_t x[3];
	ttspl_t y[2];

	ttspl_t zx[2];
	ttspl_t zy[2];
	int zi;

#ifndef HAVE_FPU
	ttspl_t error_feedback; //!< First order error feedback
#endif
} tt_biquad_t;

void tt_biquad_init(tt_biquad_t *bq, tt_context_t *ctx);
static inline void tt_biquad_finalize(tt_biquad_t *bq) {}
tt_biquad_t *tt_biquad_new(tt_context_t *ctx);
void tt_biquad_delete(tt_biquad_t *bq);

ttspl_t tt_biquad_step(tt_biquad_t *bq, ttspl_t spl);
void tt_biquad_process(tt_biquad_t *bq, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);
void tt_biquad_flush(tt_biquad_t *bq);

void tt_biquad_lowpass(tt_biquad_t *bq, int shfreq, ttspl_t q);
void tt_biquad_highpass(tt_biquad_t *bq, int shfreq, ttspl_t q);
void tt_biquad_bandpass(tt_biquad_t *bq, int cfreq, ttspl_t q);
void tt_biquad_bandstop(tt_biquad_t *bq, int cfreq, ttspl_t q);
void tt_biquad_allpass(tt_biquad_t *bq, int csfreq, ttspl_t q);
void tt_biquad_peakingeq(tt_biquad_t *bq, int cfreq, int dbgain, ttspl_t q);
void tt_biquad_lowshelf(tt_biquad_t *bq, int shfreq, int dbgain, ttspl_t q);
void tt_biquad_highshelf(tt_biquad_t *bq, int shfreq, int dbgain, ttspl_t q);

char *tt_biquad_tostring(tt_biquad_t *bq);

#endif // TT_BIQUAD_H_
