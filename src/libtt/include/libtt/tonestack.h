/*
 * tonestack.h
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

#ifndef TT_TONESTACK_H_
#define TT_TONESTACK_H_

#include "libtt/sample.h"
#include "libtt/util.h"

typedef enum {
	TT_TONESTACK_DIGITAL
} tt_tonestack_model_t;

typedef enum {
	TT_TONESTACK_CONTROL_MIN = TT_BASE2TAG(TT_TONESTACK_BASE),
	TT_TONESTACK_CONTROL_MODEL = TT_TONESTACK_CONTROL_MIN,
	TT_TONESTACK_CONTROL_BASS,
	TT_TONESTACK_CONTROL_MID,
	TT_TONESTACK_CONTROL_TREBLE,
	TT_TONESTACK_CONTROL_GAIN,
	TT_TONESTACK_CONTROL_MAX
} tt_tonestack_control_t;

typedef struct {
	tt_context_t *ctx;

	ttspl_t controls[TT_TAG2ID(TT_TONESTACK_CONTROL_MAX)];

	tt_biquad_t trebleshelf;
	tt_biquad_t bassshelf;
	ttspl_t gain;
} tt_tonestack_t;

void tt_tonestack_init(tt_tonestack_t *ts, tt_context_t *ctx);
void tt_tonestack_finalize(tt_tonestack_t *ts);
tt_tonestack_t *tt_tonestack_new(tt_context_t *ctx);
void tt_tonestack_delete(tt_tonestack_t *ts);

void tt_tonestack_setup(tt_tonestack_t *ts, tt_tonestack_model_t model);
ttspl_t tt_tonestack_get_control(tt_tonestack_t *ts, tt_tonestack_control_t ctrl);
void tt_tonestack_set_control(tt_tonestack_t *ts, tt_tonestack_control_t ctrl, ttspl_t val);
tt_tonestack_control_t tt_tonestack_enum_control(tt_tonestack_control_t ctrl);

ttspl_t tt_tonestack_step(tt_tonestack_t *ts, ttspl_t spl);
void tt_tonestack_process(tt_tonestack_t *ts, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_TONESTACK_H_
