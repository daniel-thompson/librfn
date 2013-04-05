/*
 * revert.h
 *
 * Part of libtt (the integer amplifier library)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef TT_REVERB_H_
#define TT_REVERB_H_

#include "libtt/sample.h"
#include "libtt/util.h"

typedef enum {
	TT_REVERB_CONTROL_MIN = TT_BASE2TAG(TT_REVERB_BASE),
	TT_REVERB_CONTROL_LEVEL = TT_REVERB_CONTROL_MIN,
	TT_REVERB_CONTROL_MAX
} tt_reverb_control_t;

typedef struct {
	tt_delay_t delay;
	ttspl_t gainb;
	ttspl_t gainf;
} tt_delay_allpass_t;

typedef struct {
	tt_delay_t delay;
	ttspl_t gainb;
} tt_delay_comb_t;

typedef struct {
	tt_context_t *ctx;

	ttspl_t controls[TT_TAG2ID(TT_REVERB_CONTROL_MAX)];

	ttspl_t pregain;
	tt_delay_allpass_t allpass[3];
	tt_delay_comb_t comb[4];

	ttspl_t allpass347[347];
	ttspl_t allpass113[113];
	ttspl_t allpass37[37];
	ttspl_t comb1601[1601];
	ttspl_t comb1867[1867];
	ttspl_t comb2053[2035];
	ttspl_t comb2251[2251];
} tt_reverb_t;

void tt_reverb_init(tt_reverb_t *p, tt_context_t *ctx);
void tt_reverb_finalize(tt_reverb_t *p);
tt_reverb_t *tt_reverb_new(tt_context_t *ctx);
void tt_reverb_delete(tt_reverb_t *p);

void tt_reverb_setup(tt_reverb_t *p);
ttspl_t tt_reverb_get_control(tt_reverb_t *p, tt_reverb_control_t ctrl);
void tt_reverb_set_control(tt_reverb_t *p, tt_reverb_control_t ctrl, ttspl_t val);
tt_reverb_control_t tt_reverb_enum_control(tt_reverb_control_t ctrl);

ttspl_t tt_reverb_step(tt_reverb_t *p, ttspl_t spl);
void tt_reverb_process(tt_reverb_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_REVERB_H_
