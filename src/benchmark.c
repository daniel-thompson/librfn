/*
 * benchmark.c
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

static fibre_t *next_action;

typedef struct {
	uint32_t start_time;
	uint32_t end_time;
	unsigned int cycles;
	unsigned int count;
	fibre_t fibre;
} benchmark_fibre_t;

int yield_fibre(fibre_t *fibre)
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

benchmark_fibre_t single_yield = {
	.cycles = 1000000,
	.fibre = FIBRE_VAR_INIT(yield_fibre)
};

benchmark_fibre_t paired_yield[2] = {
	{
		.cycles = 500000,
		.fibre = FIBRE_VAR_INIT(yield_fibre)
	},
	{
		.cycles = 500000,
		.fibre = FIBRE_VAR_INIT(yield_fibre)
	}
};

typedef struct {
	stats_t single;
	stats_t paired;
} benchmark_results_t;

int benchmark_once(pt_t *pt, benchmark_results_t *results)
{
	PT_BEGIN(pt);

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

typedef struct {
	fibre_t fibre;
	pt_t spawnee;;
	benchmark_results_t results;
} conductor_fibre_t;

int conductor_fibre(fibre_t *fibre)
{
	conductor_fibre_t *c = containerof(fibre, conductor_fibre_t, fibre);
	static uint64_t end_time;
	static int i;

	PT_BEGIN_FIBRE(&c->fibre);

	stats_init(&c->results.single);
	stats_init(&c->results.paired);

	/* busy wait for two seconds to try and open up the cpufreq governor */
	printf("Preparing ...");
	fflush(stdout);
	end_time = time64_now() + 2000000;
	while (time64_now() < end_time)
		PT_YIELD();
	printf(" done\n");

	printf("Benchmarking ");
	fflush(stdout);
	for (i=0; i<100; i++) {
		if (i%2 == 0) {
			printf(".");
			fflush(stdout);
		}
		PT_SPAWN(&c->spawnee, benchmark_once(&c->spawnee, &c->results));
	}
	printf(" done\n\n");

	printf("Test                   Min      Mean       Max\n");
	printf("----------------------------------------------\n");
	printf("Single          %10d%10d%10d\n", c->results.single.min,
	       stats_mean(&c->results.single), c->results.single.max);
	printf("Paired          %10d%10d%10d\n", c->results.paired.min,
	       stats_mean(&c->results.paired), c->results.paired.max);

	exit(0);
	PT_END();
}

conductor_fibre_t conductor = {
	.fibre = FIBRE_VAR_INIT(conductor_fibre)
};

int main(int argc, char *argv[])
{
	next_action = &conductor.fibre;
	fibre_run(&conductor.fibre);
	fibre_scheduler_main_loop();

	return 0;
}
