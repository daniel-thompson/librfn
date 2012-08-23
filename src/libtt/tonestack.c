/*
 * tonestack.c
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
#include "libtt/tonestack.h"

void tt_tonestack_init(tt_tonestack_t *ts, tt_context_t *ctx)
{
	memset(ts, 0, sizeof(*ts));
	ts->ctx = ctx;

	tt_biquad_init(&ts->trebleshelf, ctx);
	tt_biquad_init(&ts->bassshelf, ctx);
	ts->gain = TTINT(1);
}

void tt_tonestack_finalize(tt_tonestack_t *ts)
{
}

tt_generic_new(tonestack);
tt_generic_delete(tonestack);

void tt_tonestack_setup(tt_tonestack_t *ts, tt_tonestack_model_t model)
{
	// the defaults (all zeros) are actually about right
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_MODEL, TT_TONESTACK_DIGITAL);
}

ttspl_t tt_tonestack_get_control(tt_tonestack_t *ts, tt_tonestack_control_t ctrl)
{
	assert(ctrl >= TT_TONESTACK_CONTROL_MIN && ctrl < TT_TONESTACK_CONTROL_MAX);
	return ts->controls[TT_TAG2ID(ctrl)];
}

void tt_tonestack_set_control(tt_tonestack_t *ts, tt_tonestack_control_t ctrl, ttspl_t val)
{
	assert(ctrl >= TT_TONESTACK_CONTROL_MIN && ctrl < TT_TONESTACK_CONTROL_MAX);
	ts->controls[TT_TAG2ID(ctrl)] = val;

	// apply the change
	ttspl_t gain = ts->controls[TT_TAG2ID(TT_TONESTACK_CONTROL_GAIN)];
	ttspl_t bass = ts->controls[TT_TAG2ID(TT_TONESTACK_CONTROL_BASS)];
	ttspl_t mid = ts->controls[TT_TAG2ID(TT_TONESTACK_CONTROL_MID)];
	ttspl_t treble = ts->controls[TT_TAG2ID(TT_TONESTACK_CONTROL_TREBLE)];

	// mid takes effect solely by influencing other controls
	gain = TTADD(gain, mid);
	bass = TTSUB(bass, mid);
	treble = TTSUB(treble, mid);

	// design the filters and convert the gain to linear
	tt_biquad_highshelf(&ts->trebleshelf, 1000, TTASINT(treble), TTFLOAT(0.8));
	tt_biquad_lowshelf(&ts->bassshelf, 300, TTASINT(bass), TTFLOAT(0.8));
	ts->gain = TTDB2LINEAR(gain);
}

tt_generic_enum_control(tonestack, TT_TONESTACK_CONTROL_MIN, TT_TONESTACK_CONTROL_MAX);


ttspl_t tt_tonestack_step(tt_tonestack_t *ts, ttspl_t spl)
{
	spl = tt_biquad_step(&ts->trebleshelf, spl);
	spl = tt_biquad_step(&ts->bassshelf, spl);
	spl = TTMAL(spl, ts->gain);

	return spl;
}

tt_generic_process(tonestack);
