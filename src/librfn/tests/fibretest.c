/*
 * fibretest.c
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
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

typedef struct {
	uint32_t time;
	uint32_t max_time;
	uint32_t step;
	fibre_t fibre;
} sleep_fibre_t;

int sleep_fibre(fibre_t *f)
{
	sleep_fibre_t *s = containerof(f, sleep_fibre_t, fibre);

	PT_BEGIN_FIBRE(f);

	while (s->time < s->max_time) {
		s->time += s->step;
		PT_WAIT_UNTIL(fibre_timeout(s->time));
	}

	PT_END();
}

typedef struct {
	uint32_t count;
	uint32_t max_count;
	fibre_t fibre;
} yield_fibre_t;

int yield_fibre(fibre_t *f)
{
	yield_fibre_t *s = containerof(f, yield_fibre_t, fibre);

	PT_BEGIN_FIBRE(f);

	while (s->count < s->max_count) {
		s->count++;
		PT_YIELD();
	}

	PT_END();
}

sleep_fibre_t sleeper = {
		.max_time = 50,
		.step = 10,
		.fibre = FIBRE_VAR_INIT(sleep_fibre)
};

yield_fibre_t yielder = {
		.count = 0,
		.max_count = 5,
		.fibre = FIBRE_VAR_INIT(yield_fibre)
};

int main()
{
	fibre_run(&sleeper.fibre);

	/* only one fibre is running, it sleeps making system go idle */
	assert(10 == fibre_schedule_next(0)); // sleeper runs
	assert(10 == sleeper.time && 0 == yielder.count);
	assert(20 == fibre_schedule_next(10)); // sleeper runs
	assert(20 == sleeper.time && 0 == yielder.count);

	/* two fibres are running, one sleeps, the other yields so system
	 * remains busy
	 */
	fibre_run(&yielder.fibre);
	assert(20 == fibre_schedule_next(20)); // yielder runs, sleeper runnable
	assert(20 == sleeper.time && 1 == yielder.count);
	assert(21 == fibre_schedule_next(21)); // sleeper runs
	assert(30 == sleeper.time && 1 == yielder.count);
	assert(22 == fibre_schedule_next(22)); // yielder runs
	assert(30 == sleeper.time && 2 == yielder.count);
	assert(31 == fibre_schedule_next(31)); // yielder runs, sleeper runnable
	assert(30 == sleeper.time && 3 == yielder.count);
	assert(42 == fibre_schedule_next(42)); // sleeper runs twice
	assert(50 == sleeper.time && 3 == yielder.count);
	assert(44 == fibre_schedule_next(44)); // yielder runs
	assert(50 == sleeper.time && 4 == yielder.count);
	assert(45 == fibre_schedule_next(45)); // yielder runs
	assert(50 == sleeper.time && 5 == yielder.count);
	assert(50 == fibre_schedule_next(46)); // yielder completes
	//    ^^^^ note difference in idle time
	assert(50 == sleeper.time && 5 == yielder.count);
	assert(50 == fibre_schedule_next(47)); // idle
	assert(50 == sleeper.time && 5 == yielder.count);
	assert(50+FIBRE_UNBOUNDED_SLEEP == fibre_schedule_next(50)); // sleeper runs
	assert(50 == sleeper.time && 5 == yielder.count);

	return 0;
}
