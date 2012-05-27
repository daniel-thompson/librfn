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

typedef struct {
	ttspl_t x[3];
	ttspl_t y[2];

	ttspl_t zx[2];
	ttspl_t zy[2];
	int zi;

#ifdef HAVE_FPU
	ttspl_t error_feedback; //!< First order error feedback
#endif
} tt_biquad_t;

tt_biquad_t *tt_biquad_new();
void tt_biquad_free(tt_biquad_t *bq);

ttspl_t tt_biquad_step(tt_biquad_t *bq, ttspl_t spl);
void tt_biquad_process(tt_biquad_t *bq, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);
void tt_biquad_flush(tt_biquad_t *bq);

void tt_biquad_lowpass(tt_biquad_t *bq, int sfreq, int shfreq, ttspl_t q);
void tt_biquad_highpass(tt_biquad_t *bq, int sfreq, int shfreq, ttspl_t q);
void tt_biquad_bandpass(tt_biquad_t *bq, int sfreq, int cfreq, ttspl_t q);
void tt_biquad_bandstop(tt_biquad_t *bq, int sfreq, int cfreq, ttspl_t q);
void tt_biquad_allpass(tt_biquad_t *bq, int csfreq, ttspl_t q);
void tt_biquad_peakingeq(tt_biquad_t *bq, int sfreq, int cfreq, int dbgain, ttspl_t q);
void tt_biquad_lowshelf(tt_biquad_t *bq, int sfreq, int shfreq, int dbgain, ttspl_t q);
void tt_biquad_highshelf(tt_biquad_t *bq, int sfreq, int shfreq, int dbgain, ttspl_t q);

#endif // TT_BIQUAD_H_
