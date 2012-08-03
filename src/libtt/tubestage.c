/*
 * tubestage.c
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

#include "librfn.h"
#include "libtt.h"

struct tubetable {
	int min_Vi;
	int max_Vi;
	unsigned int mapping;
	unsigned int table_size;
	ttspl_t table[1001];
};

#include "12AX7.h"

static void setup_tube(tt_tubestage_t *ts, struct tubetable *table)
{
	// TODO: these should be looked up from the tube model
	ts->Vp = TTINT(250);
	ts->Rp = 100000;

	tt_waveshaper_setup(&ts->tube,
			TTINT(table->min_Vi), TTINT(table->max_Vi),
			table->table, lengthof(table->table));
}

void tt_tubestage_init(tt_tubestage_t *ts, tt_context_t *ctx)
{
	memset(ts, 0, sizeof(*ts));

	tt_biquad_init(&ts->input_filter, ctx);
	tt_waveshaper_init(&ts->tube);
	tt_biquad_init(&ts->feedback_filter, ctx);
	tt_dcblocker_init(&ts->output_filter, ctx);
}

void tt_tubestage_finalize(tt_tubestage_t *ts)
{
	tt_biquad_finalize(&ts->input_filter);
	//tt_waveshaper_finalize(&ts->tube);
	tt_biquad_finalize(&ts->feedback_filter);
	tt_dcblocker_finalize(&ts->output_filter);
}

tt_generic_new(tubestage);
tt_generic_delete(tubestage);

void tt_tubestage_setup(tt_tubestage_t *ts,
		tt_tubestage_tube_t tube,
		int dbgain,
		unsigned int Rk,
		unsigned int input_cutoff,
		unsigned int feedback_cutoff)
{
	struct tubetable *tubetable;

	switch (tube) {
#define C(x) \
	case TT_TUBESTAGE_##x: \
		tubetable = &tubetable_##x; \
		break
	C(12AX7_Ri68K);
	C(12AX7_Ri250K);
	default:
		assert(0);
		break;
#undef C
	}
	setup_tube(ts, tubetable);

	ts->gain = TTDB2LINEAR(TTINT(dbgain));

	ts->Rk = Rk;
	ts->predivide = TTDINT(TTINT(ts->Rk), ts->Rp);

	tt_biquad_lowpass(&ts->input_filter, input_cutoff, TTFLOAT(0.7));
	tt_biquad_lowpass(&ts->feedback_filter, feedback_cutoff, TTFLOAT(0.7));
	tt_dcblocker_setup(&ts->output_filter, 31);

	// TODO: It would be good to "pre-warm" the biasing filter
}

inline ttspl_t tt_tubestage_step(tt_tubestage_t *ts, ttspl_t spl)
{
	spl = TTMAL(spl, ts->gain);
	spl = tt_biquad_step(&ts->input_filter, spl);
	spl = TTADD(spl, ts->bias);
	spl = tt_waveshaper_step(&ts->tube, spl);

	ttspl_t feedback = TTSUB(ts->Vp, spl);
	feedback = TTMAL(feedback, ts->predivide);
	feedback = tt_biquad_step(&ts->feedback_filter, feedback);
	ts->bias = feedback;

	spl = tt_dcblocker_step(&ts->output_filter, spl);
	return spl;
}

tt_generic_process(tubestage);
