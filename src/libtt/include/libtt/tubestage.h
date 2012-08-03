/*
 * tubestage.h
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

#ifndef TT_TUBESTAGE_H_
#define TT_TUBESTAGE_H_

#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

	ttspl_t Vp;
	unsigned int Rp;
	unsigned int Rk;
	ttspl_t predivide;

	ttspl_t gain;
	tt_biquad_t input_filter;
	tt_waveshaper_t tube;
	tt_biquad_t feedback_filter;
	tt_dcblocker_t output_filter;

	ttspl_t bias;
} tt_tubestage_t;

typedef enum {
	TT_TUBESTAGE_12AX7_Ri68K,
	TT_TUBESTAGE_12AX7_Ri250K,
} tt_tubestage_tube_t;

void tt_tubestage_init(tt_tubestage_t *p, tt_context_t *ctx);
void tt_tubestage_finalize(tt_tubestage_t *p);
tt_tubestage_t *tt_tubestage_new(tt_context_t *ctx);
void tt_tubestage_delete(tt_tubestage_t *p);

void tt_tubestage_setup(tt_tubestage_t *ts,
		tt_tubestage_tube_t tube,
		int dbgain,
		unsigned int Rk,
		unsigned int input_cutoff,
		unsigned int feedback_cutoff);

ttspl_t tt_tubestage_step(tt_tubestage_t *p, ttspl_t spl);
void tt_tubestage_process(tt_tubestage_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_TUBESTAGE_H_
