/*
 * jackcommon.c
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

#include <assert.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>

#include <jack/jack.h>
#include <jack/thread.h>

#include "jackcommon.h"


typedef struct {
	jack_client_t *client;
	jack_options_t options;

	unsigned int num_ports;
	unsigned int num_inputs;
	unsigned int active_ports;
	unsigned int open_ports;

	unsigned int num_frames;

	sem_t wake_app_thread;
	sem_t wake_jack_thread;
} tt_jackcommon_t;

static tt_jackcommon_t jack_ctx = {
	NULL, JackNoStartServer
};

/**
 * \todo Exit and clean up handling...
 */
static void *process_thread(void *arg)
{
	while (true) {
		jack_ctx.num_frames = jack_cycle_wait(jack_ctx.client);
		sem_post(&jack_ctx.wake_app_thread);
		sem_wait(&jack_ctx.wake_jack_thread);
		jack_cycle_signal(jack_ctx.client, 0);
	}

	return NULL;
}


jack_port_t *tt_jackcommon_new_port(tt_context_t *ctx, bool is_input)
{
	char *name;
	jack_port_t *port;

	if (0 == jack_ctx.num_ports) {
		jack_status_t status;

		jack_ctx.client = jack_client_open(
				"libtt", jack_ctx.options, &status, NULL);

		if (NULL == jack_ctx.client)
			return NULL;

		ctx->sampling_frequency = jack_get_sample_rate(jack_ctx.client);
		ctx->grain_size = jack_get_buffer_size(jack_ctx.client);

		(void) jack_set_process_thread(jack_ctx.client, process_thread, NULL);

		sem_init(&jack_ctx.wake_app_thread, 0, 0);
		sem_init(&jack_ctx.wake_jack_thread, 0, 0);
	}

	if (is_input) {
		name = xstrdup_printf("input%d", jack_ctx.num_inputs);
	} else {
		name = xstrdup_printf("output%d", jack_ctx.num_ports - jack_ctx.num_inputs);
	}

	port = jack_port_register(
			jack_ctx.client, name, JACK_DEFAULT_AUDIO_TYPE,
			(is_input ? JackPortIsInput : JackPortIsOutput), 0);
	free(name);
	if (NULL == port)
		return NULL;

	jack_ctx.num_ports++;
	if (is_input)
		jack_ctx.num_inputs++;

	return port;
}

void tt_jackcommon_delete_port(jack_port_t *port)
{
	assert(0 == jack_ctx.active_ports);

	int res = jack_port_unregister(jack_ctx.client, port);
	assert(0 == res); // can't fail if pre-conditions are met

	if (0 == --jack_ctx.num_ports)
		(void) jack_client_close(jack_ctx.client);
}

void tt_jackcommon_activate()
{
	assert(jack_ctx.active_ports < jack_ctx.num_ports);

	if (++jack_ctx.active_ports == jack_ctx.num_ports) {
		(void) jack_acquire_real_time_scheduling(pthread_self(),
				jack_client_real_time_priority(jack_ctx.client));
		jack_activate(jack_ctx.client);
	}
}

void tt_jackcommon_deactivate()
{
	assert(jack_ctx.active_ports && jack_ctx.active_ports <= jack_ctx.num_ports);

	if (jack_ctx.active_ports-- == jack_ctx.num_ports) {
		jack_deactivate(jack_ctx.client);
		(void) jack_drop_real_time_scheduling(pthread_self());
	}
}

unsigned int tt_jackcommon_cycle_wait()
{
	assert(jack_ctx.open_ports < jack_ctx.active_ports);

	if (0 == jack_ctx.open_ports++)
		sem_wait(&jack_ctx.wake_app_thread);

	return jack_ctx.num_frames;
}

void tt_jackcommon_cycle_signal()
{
	assert(jack_ctx.open_ports && jack_ctx.open_ports <= jack_ctx.active_ports);

	if (0 == --jack_ctx.open_ports)
		sem_post(&jack_ctx.wake_jack_thread);
}
