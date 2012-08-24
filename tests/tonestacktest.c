/*
 * tonestacktest.c
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

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <librfn.h>
#include <libtt.h>

static ttspl_t tonestack_stimulate(
		tt_tonestack_t *ts, int gfreq)
{
	tt_sbuf_t *inbuf = tt_sbuf_new(1024);
	assert(inbuf);

	tt_sbuf_t *outbuf = tt_sbuf_new(1024);
	assert(outbuf);

	tt_siggen_t *sg = tt_siggen_new(ts->ctx); // use the same context as the tonestack
	assert(sg);
	tt_siggen_setup(sg, gfreq, TTFLOAT(1.570793), TT_SIGGEN_SIN);

	// precheck that we are outputing at the right level
	tt_siggen_process(sg, inbuf);
	//ttspl_t precheck = tt_analyse_rectify(inbuf);
	//assert(fuzzcmpf(1, TTASFLOAT(precheck), 1.02));

	// stimulate the filter
	for (int i=0; i<10; i++) {
		tt_siggen_process(sg, inbuf);
		tt_tonestack_process(ts, inbuf, outbuf);
	}

	// check the result
	ttspl_t result = tt_analyse_rectify(outbuf);

	// clean up
	tt_siggen_delete(sg);
	tt_sbuf_delete(outbuf);
	tt_sbuf_delete(inbuf);

	return result;
}

static bool test_response_at(tt_tonestack_t *ts, int gfreq, float db)
{
	ttspl_t level = tonestack_stimulate(ts, gfreq);
	bool ok;

	float levelf = TTASFLOAT(TTLINEAR2DB(level));

	// OK if level is correct +-2dB
	ok = fuzzcmpef(levelf, db, 2);

	printf("    %4dHz@%dHz    %6.2fdB %c= %6.2fdB            [ %s ]\n",
			gfreq, ts->ctx->sampling_frequency, levelf,
			ok ? '~' : '!', db, ok ? " OK " : "FAIL");

	return ok;
}

int main()
{
	tt_context_t *ctx = tt_context_new();
	ctx->sampling_frequency = 48000;


	tt_tonestack_t *ts = tt_tonestack_new(ctx);
	assert(ts);
	tt_tonestack_setup(ts, TT_TONESTACK_DIGITAL);

	// TEST: Stays at zero without stimulus
	assert(TTINT(0) == tt_tonestack_step(ts, TTINT(0)));
	assert(TTINT(0) == tt_tonestack_step(ts, TTINT(0)));
	assert(TTINT(0) == tt_tonestack_step(ts, TTINT(0)));

	// TEST: Should be flat by default
	assert(test_response_at(ts, 100, 0));
	assert(test_response_at(ts, 270, 0));
	assert(test_response_at(ts, 300, 0));
	assert(test_response_at(ts, 330, 0));
	assert(test_response_at(ts, 550, 0));
	assert(test_response_at(ts, 900, 0));
	assert(test_response_at(ts, 1000, 0));
	assert(test_response_at(ts, 1100, 0));
	assert(test_response_at(ts, 1500, 0));

	//
	// For the digital tonestack the logarithmic mid-point between the
	// bass corner frequency (300) and the treble corner frequency (1000)
	// is approximately 550Hz. However the bass and treble filters are tuned
	// to that this midpoint is slightly affected by bass/treble changes.
	// For that reason the target volumes at 550Hz don't strictly follow the value
	// of the "mid" control.
	//

	// TEST: drop the gain
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_GAIN, TTINT(-24));
	assert(test_response_at(ts, 100, -24));
	assert(test_response_at(ts, 550, -24));
	assert(test_response_at(ts, 3000, -24));

	// TEST: increase the gain
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_GAIN, TTINT(24));
	assert(test_response_at(ts, 100, 24));
	assert(test_response_at(ts, 550, 24));
	assert(test_response_at(ts, 3000, 24));

	// SETUP: restore default value
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_GAIN, TTINT(0));

	// TEST: remove the bass
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_BASS, TTINT(-24));
	assert(test_response_at(ts, 100, -24));
	assert(test_response_at(ts, 550, -2));
	assert(test_response_at(ts, 1000, 0));
	assert(test_response_at(ts, 3000, 0));

	// TEST: boost the bass
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_BASS, TTINT(24));
	assert(test_response_at(ts, 100, 24));
	assert(test_response_at(ts, 550, 2));
	assert(test_response_at(ts, 1000, 0));
	assert(test_response_at(ts, 3000, 0));

	// SETUP: restore default value
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_BASS, TTINT(0));

	// TEST: scoop out the middle
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_MID, TTINT(-24));
	assert(test_response_at(ts, 100, 0));
	assert(test_response_at(ts, 550, -18));
	assert(test_response_at(ts, 3000, 0));

	// TEST: boost the middle
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_MID, TTINT(24));
	assert(test_response_at(ts, 100, 0));
	assert(test_response_at(ts, 550, 18)); // 550Hz is still affect by roll-off
	assert(test_response_at(ts, 3000, 0));

	// TEST: scoop out the middle
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_MID, TTINT(-12));
	assert(test_response_at(ts, 100, 0));
	assert(test_response_at(ts, 550, -10));
	assert(test_response_at(ts, 3000, 0));

	// TEST: boost the middle
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_MID, TTINT(12));
	assert(test_response_at(ts, 100, 0));
	assert(test_response_at(ts, 550, 10));
	assert(test_response_at(ts, 3000, 0));

	// SETUP: restore default value
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_MID, TTINT(0));

	// TEST: remove the treble
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_TREBLE, TTINT(-24));
	assert(test_response_at(ts, 100, 0));
	assert(test_response_at(ts, 300, 0));
	assert(test_response_at(ts, 550, -2));
	assert(test_response_at(ts, 3000, -24));

	// TEST: boost the treble
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_TREBLE, TTINT(24));
	assert(test_response_at(ts, 100, 0));
	assert(test_response_at(ts, 300, 0));
	assert(test_response_at(ts, 550, 2));
	assert(test_response_at(ts, 3000, 24));

	// SETUP: restore default value
	tt_tonestack_set_control(ts, TT_TONESTACK_CONTROL_TREBLE, TTINT(0));

	// TIDY
	tt_tonestack_delete(ts);
	tt_context_delete(ctx);

	return 0;
}
