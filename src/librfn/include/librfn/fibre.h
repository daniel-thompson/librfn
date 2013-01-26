/*
 * fibre.h
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

#ifndef RF_FIBRE_H_
#define RF_FIBRE_H_

#include <stdint.h>

#include "list.h"
#include "messageq.h"
#include "protothreads.h"

typedef enum {
	FIBRE_STATE_YIELDED = PT_YIELDED,
	FIBRE_STATE_WAITING = PT_WAITING,
	FIBRE_STATE_EXITED = PT_EXITED,
	FIBRE_STATE_RUNNING,

	FIBRE_STATE_QUEUED = 0x10,

	FIBRE_STATE_TIMER_WAITING = FIBRE_STATE_WAITING | FIBRE_STATE_QUEUED,
	FIBRE_STATE_RUNNABLE = FIBRE_STATE_RUNNING | FIBRE_STATE_QUEUED
} fibre_state_t;

#define FIBRE_UNBOUNDED_SLEEP ((uint32_t) 0x7fffffff)

struct fibre;
typedef int fibre_entrypoint_t(struct fibre *);
typedef struct fibre {
	fibre_entrypoint_t *fn;
	uint16_t state;
	uint16_t priv;
	uint32_t duetime;
	list_node_t link;
} fibre_t;
#define FIBRE_VAR_INIT(fn) { (fn), 0, 0, 0, LIST_NODE_VAR_INIT }

typedef struct {
	fibre_t fibre;
	messageq_t eventq;
} fibre_eventq_t;
#define FIBRE_EVENTQ_VAR_INIT(fn, basep, base_len, msg_len) \
	{ \
		.fibre = FIBRE_VAR_INIT(fn), \
		.eventq = MESSAGEQ_VAR_INIT(basep, base_len, msg_len) \
	}

#define PT_BEGIN_FIBRE(f) PT_BEGIN(&((f)->priv))

fibre_t *fibre_self();
uint32_t fibre_scheduler_next(uint32_t time);

void fibre_init(fibre_t *f, fibre_entrypoint_t *fn);
void fibre_run(fibre_t *f);
bool fibre_run_atomic(fibre_t *f);
bool fibre_kill(fibre_t *f);

bool fibre_timeout(uint32_t duetime);

void fibre_eventq_init(fibre_eventq_t *evtq, fibre_entrypoint_t *fn,
		void *basep, size_t base_len, size_t msg_len);
void *fibre_eventq_claim(fibre_eventq_t *evtq);
bool fibre_eventq_send(fibre_eventq_t *evtq, void *evtp);
bool fibre_eventq_empty(fibre_eventq_t *evtq);
void *fibre_eventq_receive(fibre_eventq_t *evtq);
void fibre_eventq_release(fibre_eventq_t *evtq, void *evtp);

/*!
 * Enter the scheduler main loop.
 *
 * This function will dispatch threads when they become runnable and
 * may attempt to conserve power by going idle in some implementation
 * defined manner.
 *
 * An implementation of this function must be provided for each execution
 * environment. librfn only comes with an implementation for a small
 * subset of environments and may have to be provided by the library user.
 * POSIX environments are, however, already supported to allow the
 * demonstation programs to work.
 */
void fibre_scheduler_main_loop();

#endif // RF_FIBRE_H_
