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
		tt_biquad_t *bq, int sfreq, int gfreq)
{
	tt_sbuf_t *inbuf = tt_sbuf_new(1024);
	assert(inbuf);

	tt_sbuf_t *outbuf = tt_sbuf_new(1024);
	assert(outbuf);

	tt_siggen_t *sg = tt_siggen_new();
	assert(sg);
	tt_siggen_setup(sg, sfreq, gfreq, 1.570793, TT_SIGGEN_SIN);

	// precheck that we are outputing at the right level
	tt_siggen_process(sg, inbuf);
	ttspl_t precheck = tt_analyse_rectify(inbuf);
	assert(fuzzcmpf(1, TTASFLOAT(precheck), 1.01));

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

int main()
{
	char *s;
	float minus3, minus12, unity;

	// SETUP: 400Hz low pass filter
	printf("Low-pass filter\n");
	tt_biquad_t *bq = tt_biquad_new();
	assert(bq);
	tt_biquad_lowpass(bq, 48000, 400, TTFLOAT(0.7));
	s = tt_biquad_tostring(bq);
	assert(s);
	printf("%s\n", s);
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

	// TEST: Check the shoulder frequency is -3dB
	minus3 = TTASFLOAT(TTLINEAR2DB(biquad_stimulate(bq, 48000, 400)));
	printf("  minus3 = %5.2fdB\n", minus3);
	assert(fuzzcmpf(-3, minus3, 1.05));

	// TEST: Check for rolloff at ~12dB per octave
	minus12 = TTASFLOAT(TTLINEAR2DB(biquad_stimulate(bq, 48000, 800)));
	printf("  minus12 = %5.2fdB\n", minus12);
	assert(fuzzcmpf(-12, minus12, 1.05));

	// TEST: Check for full signal in passband
	unity = TTASFLOAT(biquad_stimulate(bq, 48000, 200));
	printf("  unity = %9.6f\n", unity);
	assert(fuzzcmpf(1, unity, 1.05));

	// SETUP: 600Hz high pass filter
	printf("High-pass filter\n");
	tt_biquad_highpass(bq, 48000, 600, TTFLOAT(0.7));

	// TEST: Check the shoulder frequency is -3dB
	minus3 = TTASFLOAT(TTLINEAR2DB(biquad_stimulate(bq, 48000, 600)));
	printf("  minus3 = %5.2fdB\n", minus3);
	assert(fuzzcmpf(-3, minus3, 1.05));

	// TEST: Check for rolloff at ~12dB per octave
	minus12 = TTASFLOAT(TTLINEAR2DB(biquad_stimulate(bq, 48000, 300)));
	printf("  minus12 = %5.2fdB\n", minus12);
	assert(fuzzcmpf(-12, minus12, 1.05));

	// TEST: Check for full signal in passband
	unity = TTASFLOAT(biquad_stimulate(bq, 48000, 1200));
	printf("  unity = %9.6f\n", unity);
	assert(fuzzcmpf(1, unity, 1.05));

	return 0;
}
