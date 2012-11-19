/*
 * benchmark.c
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
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <librfn.h>
#include <libtt.h>


int main()
{
	struct timespec start;
	struct timespec end;
	double ratio;

	// INIT
	tt_context_t *ctx = tt_context_new();
	ctx->sampling_frequency = 48000;
	tt_sbuf_t *inbuf = tt_sbuf_new(ctx->grain_size);
	tt_sbuf_t *outbuf = tt_sbuf_new(ctx->grain_size);
	tt_siggen_t *sg = tt_siggen_new(ctx);
	tt_tintamp_t *tt = tt_tintamp_new(ctx);
	tt_drummachine_t *dm = tt_drummachine_new(ctx);

	// generate an input buffer
	tt_siggen_setup(sg, 0, TTFLOAT(0.8), TT_SIGGEN_WHITE_NOISE);
	tt_siggen_process(sg, inbuf);
	tt_drummachine_setup(dm);

	//
	// tintamp benchmark
	//

	// preheat things a bit (both to get the caches warm and CPU frequency scaling to
	// kick in)
	for (int i=0; i<((2*ctx->sampling_frequency)/ctx->grain_size); i++)
		tt_tintamp_process(tt, inbuf, outbuf);

	// do the benchmark
	clock_gettime(CLOCK_REALTIME, &start);
	for (int i=0; i<((20*ctx->sampling_frequency)/ctx->grain_size); i++)
		tt_tintamp_process(tt, inbuf, outbuf);
	clock_gettime(CLOCK_REALTIME, &end);

	ratio = (end.tv_nsec - start.tv_nsec);
	ratio += (end.tv_sec - start.tv_sec) * 1000000000.0;
	printf("tintamp - Execution time: %.3fms\n", ratio/1000000.0);

	// convert to CPU usage
	ratio /= 20000000000.0;
	printf("tintamp - CPU usage:      %5.2f%%\n", ratio * 100.0);

	//
	// drummachine benchmark
	//

	// preheat things a bit (both to get the caches warm and CPU frequency scaling to
	// kick in)
	for (int i=0; i<((2*ctx->sampling_frequency)/ctx->grain_size); i++)
		tt_drummachine_process(dm, outbuf);

	// do the benchmark
	clock_gettime(CLOCK_REALTIME, &start);
	for (int i=0; i<((20*ctx->sampling_frequency)/ctx->grain_size); i++)
		tt_drummachine_process(dm, outbuf);
	clock_gettime(CLOCK_REALTIME, &end);

	ratio = (end.tv_nsec - start.tv_nsec);
	ratio += (end.tv_sec - start.tv_sec) * 1000000000.0;
	printf("drummachine - Execution time: %.3fms\n", ratio/1000000.0);

	// convert to CPU usage
	ratio /= 20000000000.0;
	printf("drummachine - CPU usage:      %5.2f%%\n", ratio * 100.0);

	// TIDY
	tt_drummachine_delete(dm);
	tt_tintamp_delete(tt);
	tt_siggen_delete(sg);
	tt_sbuf_delete(outbuf);
	tt_sbuf_delete(inbuf);
	tt_context_delete(ctx);

	return 0;
}
