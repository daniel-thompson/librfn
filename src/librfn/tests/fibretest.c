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

typedef struct {
	int id;
} event_descriptor_t;

typedef struct {
	event_descriptor_t last_event;
	fibre_eventq_t evtq;
} eventq_t;

int event_handler(fibre_t *f)
{
	fibre_eventq_t *fevtq = containerof(f, fibre_eventq_t, fibre);
	eventq_t *evtq = containerof(fevtq, eventq_t, evtq);

	PT_BEGIN_FIBRE(f);

	while (true) {
		event_descriptor_t *evt;

		PT_WAIT_UNTIL(NULL != (evt = fibre_eventq_receive(fevtq)));
		evtq->last_event = *evt;
		fibre_eventq_release(fevtq, evt);
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

event_descriptor_t events[8];
eventq_t handler = {
		.evtq = FIBRE_EVENTQ_VAR_INIT(event_handler,
				events, sizeof(events), sizeof(events[0]))
};

int main()
{
	fibre_run(&sleeper.fibre);

	/* only one fibre is running, it sleeps making system go idle */
	verify(10 == fibre_schedule_next(0)); // sleeper runs
	verify(10 == sleeper.time && 0 == yielder.count);
	verify(20 == fibre_schedule_next(10)); // sleeper runs
	verify(20 == sleeper.time && 0 == yielder.count);

	/* two fibres are running, one sleeps, the other yields so system
	 * remains busy
	 */
	fibre_run(&yielder.fibre);
	verify(20 == fibre_schedule_next(20)); // yielder runs, sleeper runnable
	verify(20 == sleeper.time && 1 == yielder.count);
	verify(21 == fibre_schedule_next(21)); // sleeper runs
	verify(30 == sleeper.time && 1 == yielder.count);
	verify(22 == fibre_schedule_next(22)); // yielder runs
	verify(30 == sleeper.time && 2 == yielder.count);
	verify(31 == fibre_schedule_next(31)); // yielder runs, sleeper runnable
	verify(30 == sleeper.time && 3 == yielder.count);
	verify(42 == fibre_schedule_next(42)); // sleeper runs twice
	verify(50 == sleeper.time && 3 == yielder.count);
	verify(44 == fibre_schedule_next(44)); // yielder runs
	verify(50 == sleeper.time && 4 == yielder.count);
	verify(45 == fibre_schedule_next(45)); // yielder runs
	verify(50 == sleeper.time && 5 == yielder.count);
	verify(50 == fibre_schedule_next(46)); // yielder completes
	//    ^^^^ note difference in idle time
	verify(50 == sleeper.time && 5 == yielder.count);
	verify(50 == fibre_schedule_next(47)); // idle
	verify(50 == sleeper.time && 5 == yielder.count);
	verify(50+FIBRE_UNBOUNDED_SLEEP == fibre_schedule_next(50)); // sleeper runs
	verify(50 == sleeper.time && 5 == yielder.count);

	fibre_run(&handler.evtq.fibre);
	verify(60+FIBRE_UNBOUNDED_SLEEP ==
		     fibre_schedule_next(60)); // handler runs to wait
	assert(0 == handler.last_event.id);
	event_descriptor_t *pdesc;
	pdesc = fibre_eventq_claim(&handler.evtq);
	pdesc->id = 1;
	fibre_eventq_send(&handler.evtq, pdesc);
	verify(70+FIBRE_UNBOUNDED_SLEEP ==
		     fibre_schedule_next(70)); // handler runs to wait
	assert(1 == handler.last_event.id);


	return 0;
}
