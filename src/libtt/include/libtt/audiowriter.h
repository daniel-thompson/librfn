/*
 * audiowriter.h
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

#ifndef TT_AUDIOWRITER_H_
#define TT_AUDIOWRITER_H_

#include "libtt/sample.h"
#include "libtt/util.h"
#ifdef HAVE_ALSA
#include "libtt/alsawriter.h"
#endif
#ifdef HAVE_PULSE
#include "libtt/pulsewriter.h"
#endif

typedef struct {
	void (*finalize)(void *p);
	void (*setup)(void *p);
	ttspl_t (*step)(void *p, ttspl_t spl);
	void (*process)(void *p, tt_sbuf_t *inbuf);
} tt_audiowriter_vtable_t;

typedef struct {
	// The union must be first in order to get the following relationship:
	//   &writer == &writer.u.alsawriter == &writer.u.pulsewriter == ...
	union {
#ifdef HAVE_ALSA
		tt_alsawriter_t alsawriter;
#endif
#ifdef HAVE_PULSE
		tt_pulsewriter_t pulsewriter;
#endif
	} u;

	tt_audiowriter_vtable_t *vtable;
} tt_audiowriter_t;

void tt_audiowriter_init(tt_audiowriter_t *p, tt_context_t *ctx);
void tt_audiowriter_finalize(tt_audiowriter_t *p);
tt_audiowriter_t *tt_audiowriter_new(tt_context_t *ctx);
void tt_audiowriter_delete(tt_audiowriter_t *p);

void tt_audiowriter_setup(tt_audiowriter_t *p);

void tt_audiowriter_step(tt_audiowriter_t *p, ttspl_t spl);
void tt_audiowriter_process(tt_audiowriter_t *p, tt_sbuf_t *inbuf);

#endif // TT_AUDIOWRITER_H_
