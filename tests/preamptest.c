/*
 * preamptest.c
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

void settle(tt_preamp_t *p, tt_siggen_t *sg,
		tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	tt_siggen_setup(sg, 400, TTFLOAT(0.0), TT_SIGGEN_SIN);
	tt_siggen_process(sg, inbuf);

	// TEST: silent after settling time (~quarter of a second)
	for (int i=0; i<50; i++)
	    tt_preamp_process(p, inbuf, outbuf);

	float peak = TTASFLOAT(TTLINEAR2DB(tt_analyse_peak(outbuf)));
	float rectify = TTASFLOAT(TTLINEAR2DB(tt_analyse_rectify(outbuf)));
	printf("peak %6.2fdb  rectify %6.2fdb\n", peak, rectify);
	// TODO: This is a hack, really the settled response should be <96
	assert(peak < -90.0);
	assert(rectify < -96.0);

}

void stimulate(tt_preamp_t *p, tt_siggen_t *sg,
		tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	tt_siggen_setup(sg, 200, TTDB2LINEAR(TTINT(-12)), TT_SIGGEN_SIN);
	tt_siggen_process(sg, inbuf);
	tt_preamp_process(p, inbuf, outbuf);
	tt_siggen_process(sg, inbuf);
	tt_preamp_process(p, inbuf, outbuf);
}

int main()
{
	float peak, rectify, input;

	// INIT
	tt_context_t *ctx = tt_context_new();
	tt_sbuf_t *inbuf = tt_sbuf_new(ctx->grain_size);
	tt_sbuf_t *outbuf = tt_sbuf_new(ctx->grain_size);
	tt_siggen_t *sg = tt_siggen_new(ctx);
	tt_preamp_t *p = tt_preamp_new(ctx);

	tt_preamp_setup(p, TT_PREAMP_CLEAN);
	settle(p, sg, inbuf, outbuf);
	stimulate(p, sg, inbuf, outbuf);
	peak = TTASFLOAT(TTLINEAR2DB(tt_analyse_peak(outbuf)));
	rectify = TTASFLOAT(TTLINEAR2DB(tt_analyse_rectify(outbuf)));
	input = TTASFLOAT(TTLINEAR2DB(tt_analyse_rectify(inbuf)));
	printf("peak %7.2fdb  rectify %7.2fdb  input %7.2fdb  gain %7.2fdb\n",
			peak, rectify, input, rectify - input);
	assert((rectify - input) < 0); // no clipping
	assert((rectify - input) > -3); // not much volume loss

	// TIDY
	tt_preamp_delete(p);
	tt_siggen_delete(sg);
	tt_sbuf_delete(outbuf);
	tt_sbuf_delete(inbuf);
	tt_context_delete(ctx);

	return 0;
}
