/*
 * jacklua.c
 *
 * Part of tintamp (the integer amplifier)
 *
 * Copyright (C) 2012 Daniel Thompson <daniel@redfelineninja.org.uk> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <assert.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>

#include <jack/jack.h>

#include <libtt.h>

typedef struct {
	jack_client_t *client;

	jack_port_t *input_port;
	jack_port_t *output_port;

	tt_context_t tt_ctx;
	tt_tintamp_t tintamp;

	tt_sbuf_t *input_buf;
	tt_sbuf_t *output_buf;
} appctx_t;

appctx_t appctx;

static int process (jack_nframes_t nframes, void *arg)
{
	appctx_t *ctx = arg;

	assert(nframes == ctx->tt_ctx.grain_size);

	jack_default_audio_sample_t *in = jack_port_get_buffer (ctx->input_port, nframes);
	jack_default_audio_sample_t *out = jack_port_get_buffer (ctx->output_port, nframes);

	tt_float_to_sbuf(in, nframes, ctx->input_buf);
	tt_tintamp_process(&ctx->tintamp, ctx->input_buf, ctx->output_buf);
	tt_sbuf_to_float(ctx->output_buf, out, nframes);

	return 0;      
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
static void jack_shutdown (void *arg)
{
	exit (1);
}

static void signal_handler(int sig)
{
	appctx_t *ctx = &appctx;

	jack_client_close(ctx->client);
	fprintf(stderr, "signal received, exiting ...\n");
	exit(0);
}

int main (int argc, char *argv[])
{
	appctx_t *ctx = &appctx;

	const char *client_name;
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;

	if (argc >= 2) {		/* client name specified? */
		client_name = argv[1];
		if (argc >= 3) {	/* server name specified? */
			server_name = argv[2];
            int my_option = JackNullOption | JackServerName;
			options = (jack_options_t)my_option;
		}
	} else {			/* use basename of argv[0] */
		client_name = strrchr(argv[0], '/');
		if (client_name == 0) {
			client_name = argv[0];
		} else {
			client_name++;
		}
	}

	/* open a client connection to the JACK server */

	ctx->client = jack_client_open (client_name, options, &status, server_name);
	if (ctx->client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(ctx->client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}

	/* tell the JACK server to call `process()' whenever
	   there is work to be done.
	*/

	jack_set_process_callback (ctx->client, process, ctx);

	/* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (ctx->client, jack_shutdown, 0);

	/* create two ports */

	ctx->input_port = jack_port_register (ctx->client, "input",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	ctx->output_port = jack_port_register (ctx->client, "output",
			JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if ((ctx->input_port == NULL) || (ctx->output_port == NULL)) {
		fprintf(stderr, "no more JACK ports available\n");
		exit (1);
	}

	/* setup tintamp */
	tt_context_init(&ctx->tt_ctx);
	ctx->tt_ctx.sampling_frequency = jack_get_sample_rate(ctx->client);
	ctx->tt_ctx.grain_size = jack_get_buffer_size(ctx->client);

	tt_tintamp_init(&ctx->tintamp, &ctx->tt_ctx);

	ctx->input_buf = tt_sbuf_new(ctx->tt_ctx.grain_size);
	ctx->output_buf = tt_sbuf_new(ctx->tt_ctx.grain_size);

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (ctx->client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

        /* install a signal handler to properly quits jack client */
	signal(SIGQUIT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);

	/* keep running until the Ctrl+C */
	while (1) {
		sleep (3600);
	}

	jack_client_close (ctx->client);
	exit (0);
}
