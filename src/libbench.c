/*
 * libbench.c
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2015 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "librfn.h"
#include "libbench.h"

static fibre_t *next_action;

typedef struct {
	uint32_t start_time;
	uint32_t end_time;
	unsigned int cycles;
	unsigned int count;
	fibre_t fibre;
} benchmark_fibre_t;

static int yield_fibre(fibre_t *fibre)
{
	benchmark_fibre_t *bm = containerof(fibre, benchmark_fibre_t, fibre);

	PT_BEGIN_FIBRE(fibre);

	bm->start_time = time_now();
	bm->count = 0;

	while (bm->count++ < bm->cycles)
		PT_YIELD();

	bm->end_time = time_now();
	fibre_run(next_action);
	PT_END();
}

static benchmark_fibre_t single_yield = {
	.cycles = 1000000,
	.fibre = FIBRE_VAR_INIT(yield_fibre)
};

static benchmark_fibre_t paired_yield[2] = {
	{
		.cycles = 500000,
		.fibre = FIBRE_VAR_INIT(yield_fibre)
	},
	{
		.cycles = 500000,
		.fibre = FIBRE_VAR_INIT(yield_fibre)
	}
};

void benchmark_init(benchmark_results_t *results, fibre_t *wakeup)
{
	memset(results, 0, sizeof(*results));
	stats_init(&results->single);
	stats_init(&results->paired);
	results->wakeup = wakeup;
}	

int benchmark_run_once(benchmark_results_t *results)
{
	/* next_action is a bit of a hack but since it is meaningless to run
	 * two benchmarks concurrently it is sufficient to unconditionally
	 * update this every time we run.
	 */
	next_action = results->wakeup;

	PT_BEGIN(&results->pt);

	fibre_run(&single_yield.fibre);
	PT_WAIT();
	stats_add(&results->single,
		  single_yield.end_time - single_yield.start_time);

	fibre_run(&paired_yield[0].fibre);
	fibre_run(&paired_yield[1].fibre);
	PT_WAIT();
	stats_add(&results->paired,
		  paired_yield[1].end_time - paired_yield[0].start_time);

	PT_END();
}

const char *benchmark_get_result(benchmark_results_t *results, int n,
				 stats_t *s)
{
	switch (n) {
	case 0:
		memcpy(s, &results->single, sizeof(*s));
		return "Single";
	case 1:
		memcpy(s, &results->paired, sizeof(*s));
		return "Paired";
	default:
		return NULL;
	}
}

void benchmark_show_results(benchmark_results_t *results)
{
	const char *name;
	stats_t result;

	printf("Test                   Min      Mean       Max\n");
	printf("----------------------------------------------\n");

	for (int i = 0; (name = benchmark_get_result(results, i, &result)); i++)
		printf("%-16s%10d%10d%10d\n", name,
		       result.min, stats_mean(&result), result.max);
}	

void benchmark_show_csv(benchmark_results_t *results, FILE *f)
{
	const char *name;
	stats_t result;

	fprintf(f,"\"Test\",\"Min\",\"Mean\",\"Max\"\n");
	for (int i = 0; (name = benchmark_get_result(results, i, &result)); i++)
		fprintf(f, "\"%s\",%d,%d,%d\n", name,
		        result.min, stats_mean(&result), result.max);
}
