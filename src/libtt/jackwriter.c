/*
 * jackwriter.c
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

#include <string.h>

#include "jackcommon.h"
#include "libtt.h"
#include "libtt/jackwriter.h"


bool tt_jackwriter_init(tt_jackwriter_t *p, tt_context_t *ctx)
{
	memset(p, 0, sizeof(*p));
	p->ctx = ctx;

	p->port = tt_jackcommon_new_port(ctx, false);
	if (NULL == p->port)
		return false;

	p->buf = tt_sbuf_new(ctx->grain_size);
	if (NULL == p->buf) {
		tt_jackcommon_delete_port(p->port);
		return false;
	}

	return true;
}

void tt_jackwriter_finalize(tt_jackwriter_t *p)
{
	tt_sbuf_delete(p->buf);
	if (p->active)
		tt_jackcommon_deactivate();
	tt_jackcommon_delete_port(p->port);
}

tt_generic_new(jackwriter);
tt_generic_delete(jackwriter);

void tt_jackwriter_setup(tt_jackwriter_t *p)
{
	tt_jackcommon_activate();
}

void tt_jackwriter_process(tt_jackwriter_t *p, tt_sbuf_t *outbuf)
{
	unsigned int grain = tt_jackcommon_cycle_wait();

	assert(p->ctx->grain_size == grain);

	jack_default_audio_sample_t *out = jack_port_get_buffer(p->port, grain);
	tt_sbuf_to_float(outbuf, out, grain);

	tt_jackcommon_cycle_signal();
}

