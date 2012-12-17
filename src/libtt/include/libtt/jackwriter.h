/*
 * jackwriter.h
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

#ifndef TT_JACKWRITER_H_
#define TT_JACKWRITER_H_

#include <jack/jack.h>

#include "libtt/sample.h"
#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

	jack_port_t *port;
	bool active;

	tt_sbuf_t *buf;
} tt_jackwriter_t;

bool tt_jackwriter_init(tt_jackwriter_t *p, tt_context_t *ctx);
void tt_jackwriter_finalize(tt_jackwriter_t *p);
tt_jackwriter_t *tt_jackwriter_new(tt_context_t *ctx);
void tt_jackwriter_delete(tt_jackwriter_t *p);

void tt_jackwriter_setup(tt_jackwriter_t *p);

//void tt_jackwriter_step(tt_jackwriter_t *p, ttspl_t spl);
void tt_jackwriter_process(tt_jackwriter_t *p, tt_sbuf_t *inbuf);

#endif // TT_JACKWRITER_H_
