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
#include <stdio.h>
#include <stdlib.h>

#include <librfn.h>
#include <libtt.h>

int main(int argc, char **argv)
{
	int test_cycles = 1;

	if (argc >= 2)
		test_cycles = strtol(argv[1], (char **) NULL, 10);

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

	// Log file
	char *prefix = getenv("BENCHMARK_PREFIX");
	char *fname = xstrdup_printf("%sbenchmark.csv", prefix ? prefix : "");
	FILE *logfile = fopen(fname, "w");
	free(fname);
	assert(logfile); // test is allowed to fail if no resources

	//
	// variables needed to gather a benchmark
	//

	rf_benchmark_t bm;
	rf_benchmark_results_t results;
	double us_per_loop = (1000000.0 * ctx->grain_size) / ctx->sampling_frequency;
	double us;

	//
	// tintamp benchmark
	//

	for (int i=0; i<test_cycles; i++) {
		rf_benchmark_init(&bm, 2*1000000);
		for (us=0; rf_benchmark_running(&bm); us+=us_per_loop)
			tt_tintamp_process(tt, inbuf, outbuf);
		rf_benchmark_finalize(&bm, us, &results);
		rf_benchmark_results_show(&results, "tt_tintamp_process");
		rf_benchmark_results_as_csv(&results, "tt_tintamp_process", logfile);
	}

	//
	// drummachine benchmark
	//

	for (int i=0; i<test_cycles; i++) {
		rf_benchmark_init(&bm, 2*1000000);
		for (us=0; rf_benchmark_running(&bm); us+=us_per_loop)
			tt_drummachine_process(dm, outbuf);
		rf_benchmark_finalize(&bm, us, &results);
		rf_benchmark_results_show(&results, "tt_drummachine_process");
		rf_benchmark_results_as_csv(&results, "tt_drummachine_process", logfile);
	}

	// TIDY
	fclose(logfile);
	tt_drummachine_delete(dm);
	tt_tintamp_delete(tt);
	tt_siggen_delete(sg);
	tt_sbuf_delete(outbuf);
	tt_sbuf_delete(inbuf);
	tt_context_delete(ctx);

	return 0;
}
