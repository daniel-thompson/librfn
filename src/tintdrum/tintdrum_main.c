/*
 * tintdrum_main.c
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

#include <librfn.h>
#include <libtt.h>

typedef struct {
	tt_context_t tt_ctx;
	tt_tintdrum_t tintdrum;
	tt_audiowriter_t writer;

	tt_sbuf_t *buf;
} appctx_t;

appctx_t appctx;

void main_loop(appctx_t *ctx)
{
	sigset_t sigset;

	// block SIGINT during the main loop
	sigemptyset(&sigset);
	sigaddset(&sigset, SIGINT);
	(void) sigprocmask(SIG_BLOCK, &sigset, NULL);

	// main loop handler
	while (0 == sigpending(&sigset) && !sigismember(&sigset, SIGINT)) {
		tt_tintdrum_process(&ctx->tintdrum, ctx->buf);
		tt_audiowriter_process(&ctx->writer, ctx->buf);
	}

	// re-enable SIGINT
	sigemptyset(&sigset);
	(void) sigprocmask(SIG_SETMASK, &sigset, NULL);
	sigaddset(&sigset, SIGINT);
	(void) sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}

int main (int argc, char *argv[])
{
	appctx_t *ctx = &appctx;

	char *settings_fname;
	FILE *settings_file;
	tt_preset_t settings;

	// setup tintamp
	tt_context_init(&ctx->tt_ctx);
	tt_tintdrum_init(&ctx->tintdrum, &ctx->tt_ctx);
	tt_audiowriter_init(&ctx->writer, &ctx->tt_ctx);

	// tintdrum has no setup function
	tt_audiowriter_setup(&ctx->writer);

	// load the previous settings from file
	tt_preset_init(&settings, &tt_preset_ops_tintdrum);
	settings_fname = getenv("HOME");
	if (settings_fname)
		settings_fname = xstrdup_join(settings_fname, "/.tindrum.settings");
	else
		settings_fname = xstrdup("tintdrum.settings");
	settings_file = fopen(settings_fname, "r");
	if (settings_file) {
		tt_presetio_deserialize(settings_file, &settings);
		tt_preset_restore(&settings, &ctx->tintdrum);
		fclose(settings_file);
	}

	ctx->buf = tt_sbuf_new(ctx->tt_ctx.grain_size);

	main_loop(ctx);

	// save current settings
	settings_file = fopen(settings_fname, "w");
	free(settings_fname);
	if (settings_file) {
		tt_preset_save(&settings, &ctx->tintdrum);
		tt_presetio_serialize(settings_file, &settings);
		fclose(settings_file);
	}

	tt_tintdrum_finalize(&ctx->tintdrum);
	tt_preset_finalize(&settings);

	return 0;
}
