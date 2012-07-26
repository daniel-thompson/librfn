/*
 * biquadtest.c
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

static ttspl_t biquad_stimulate(
		tt_biquad_t *bq, int gfreq)
{
	tt_sbuf_t *inbuf = tt_sbuf_new(1024);
	assert(inbuf);

	tt_sbuf_t *outbuf = tt_sbuf_new(1024);
	assert(outbuf);

	tt_siggen_t *sg = tt_siggen_new(bq->ctx); // use the same context as the biquad
	assert(sg);
	tt_siggen_setup(sg, gfreq, 1.570793, TT_SIGGEN_SIN);

	// precheck that we are outputing at the right level
	tt_siggen_process(sg, inbuf);
	//ttspl_t precheck = tt_analyse_rectify(inbuf);
	//assert(fuzzcmpf(1, TTASFLOAT(precheck), 1.02));

	// stimulate the filter
	for (int i=0; i<10; i++) {
		tt_siggen_process(sg, inbuf);
		tt_biquad_process(bq, inbuf, outbuf);
	}

	// check the result
	ttspl_t result = tt_analyse_rectify(outbuf);

	// clean up
	tt_siggen_delete(sg);
	tt_sbuf_delete(outbuf);
	tt_sbuf_delete(inbuf);

	return result;
}

static bool test_response_at(tt_biquad_t *bq, int gfreq, float db)
{
	ttspl_t level = biquad_stimulate(bq, gfreq);
	bool ok;

	if (0 == db) {
		float levelf = TTASFLOAT(level);

		ok = fuzzcmpf(levelf, 1, 1.05);

		printf("    %4dHz@%dHz    %6.2f   %c=    1.00             [ %s ]\n",
				gfreq, bq->ctx->sampling_frequency, levelf,
				ok ? '~' : '!', ok ? " OK " : "FAIL");
	} else {
		float levelf = TTASFLOAT(TTLINEAR2DB(level));

		/* special case for very quiet signals */
		if (db <= -96)
			ok = levelf <= db;
		else
			ok = fuzzcmpf(levelf, db, 1.05);

		printf("    %4dHz@%dHz    %6.2fdB %c= %6.2fdB            [ %s ]\n",
				gfreq, bq->ctx->sampling_frequency, levelf,
				ok ? '~' : '!', db, ok ? " OK " : "FAIL");
	}

	return ok;
}

int main()
{
	char *s;

	tt_context_t *ctx = tt_context_new();
	ctx->sampling_frequency = 48000;

	// SETUP: 400Hz low pass filter
	printf("Low-pass filter (400Hz@%dHz):\n", ctx->sampling_frequency);
	tt_biquad_t *bq = tt_biquad_new(ctx);
	assert(bq);
	tt_biquad_lowpass(bq, 400, TTFLOAT(0.7));

	// TEST: Very basic test of tostring
	s = tt_biquad_tostring(bq);
	assert(s);
	assert(s[0] == 'y');
	free(s);

	// TEST: Stays at zero without stimulus
	assert(TTINT(0) == tt_biquad_step(bq, TTINT(0)));
	assert(TTINT(0) == tt_biquad_step(bq, TTINT(0)));
	assert(TTINT(0) == tt_biquad_step(bq, TTINT(0)));

	// TEST: When stimulated by an impulse the effect has a duration
	//       beyond the memory of the filter (the I in IIR)
	ttspl_t y0 = tt_biquad_step(bq, TTINT(1));
	ttspl_t y1 = tt_biquad_step(bq, TTINT(0));
	assert(TTINT(0) != y1);
	assert(TTINT(0) != tt_biquad_step(bq, TTINT(0)));
	assert(TTINT(0) != tt_biquad_step(bq, TTINT(0)));
	assert(TTINT(0) != tt_biquad_step(bq, TTINT(0)));
	assert(TTINT(0) != tt_biquad_step(bq, TTINT(0)));
	assert(TTINT(0) != tt_biquad_step(bq, TTINT(0)));

	// TEST: Flush causes the filter to forget its state
	tt_biquad_flush(bq);

	assert(TTINT(0) == tt_biquad_step(bq, TTINT(0)));

	// TEST: Flush does not damage filter coefficients
	assert(y0 == tt_biquad_step(bq, TTINT(1)));
	assert(y1 == tt_biquad_step(bq, TTINT(0)));

	// TEST: Should response is -3dB, rolloff at ~12dB per octave and
	//       full signal in passband,
	assert(test_response_at(bq, 400, -3));
	assert(test_response_at(bq, 800, -12));
	assert(test_response_at(bq, 200, 0));

	// TEST: High pass filter
	printf("High-pass filter (600Hz@%dHz):\n", ctx->sampling_frequency);
	tt_biquad_highpass(bq, 600, TTFLOAT(0.7));
	assert(test_response_at(bq, 600, -3));
	assert(test_response_at(bq, 300, -12));
	assert(test_response_at(bq, 1200, 0));

	// TEST: Band pass filter
	printf("Band-pass filter (1000Hz@%dHz):\n", ctx->sampling_frequency);
	tt_biquad_bandpass(bq, 1000, TTFLOAT(0.7));
	assert(test_response_at(bq, 1000, 0));
	assert(test_response_at(bq, 500, -3.2));
	assert(test_response_at(bq, 2000, -3.2));

	// TEST: Band stop filter
	printf("Band-stop filter (1000Hz@%dHz):\n", ctx->sampling_frequency);
	tt_biquad_bandstop(bq, 1000, TTFLOAT(0.7));
	assert(test_response_at(bq, 1000, -96));
	assert(test_response_at(bq, 500, -2.8));
	assert(test_response_at(bq, 2000, -2.8));

	// TEST: All pass
	printf("All pass filter (440Hz@%dHz):\n", ctx->sampling_frequency);
	tt_biquad_allpass(bq, 400, TTFLOAT(0.7));
	for (int f = 200; f < 10000; f *= 2) {
		assert(test_response_at(bq, f, 0));
	}

	// TEST: Peaking EQ
	printf("Peaking EQ filter (400Hz@%dHz with -16dB):\n", ctx->sampling_frequency);
	tt_biquad_peakingeq(bq, 400, -16, TTFLOAT(8.0));
	assert(test_response_at(bq, 400, -16));
	assert(test_response_at(bq, 200, 0));
	assert(test_response_at(bq, 800, 0));
	assert(test_response_at(bq, 1600, 0));

	// TEST: Peaking EQ
	printf("Peaking EQ filter (400Hz@%dHz with 6dB):\n", ctx->sampling_frequency);
	tt_biquad_peakingeq(bq, 400, 6, TTFLOAT(2.0));
	assert(test_response_at(bq, 400, 6));
	assert(test_response_at(bq, 1600, 0));

	// TEST: 750Hz high shelf filter
	printf("High-shelf filter (750Hz@%dHz):\n", ctx->sampling_frequency);
	tt_biquad_highshelf(bq, 750, -6, TTFLOAT(0.7));
	assert(test_response_at(bq, 300, 0));
	assert(test_response_at(bq, 2000, -6));

	// TEST: 750Hz high boost filter
	printf("High-boost filter (750Hz@%dHz):\n", ctx->sampling_frequency);
	tt_biquad_highshelf(bq, 750, 6, TTFLOAT(0.7));
	assert(test_response_at(bq, 300, 0));
	assert(test_response_at(bq, 2000, 6));

	// TEST: 750Hz low shelf filter
	printf("Low-shelf filter (750Hz@%dHz):\n", ctx->sampling_frequency);
	tt_biquad_lowshelf(bq, 750, -6, TTFLOAT(0.7));
	assert(test_response_at(bq, 2000, 0));
	assert(test_response_at(bq, 300, -6));

	// TIDY
	tt_biquad_delete(bq);
	tt_context_delete(ctx);

	return 0;
}
