/*
 * pulsewriter.c
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

#include "libtt.h"
#include "libtt/pulsewriter.h"

void tt_pulsewriter_init(tt_pulsewriter_t *p, tt_context_t *ctx)
{
	memset(p, 0, sizeof(*p));
	p->ctx = ctx;
}

void tt_pulsewriter_finalize(tt_pulsewriter_t *p)
{
	if (p->pulse) {
		(void) pa_simple_flush(p->pulse, NULL);
		pa_simple_free(p->pulse);
	}
}

tt_generic_new(pulsewriter);
tt_generic_delete(pulsewriter);

void tt_pulsewriter_setup(tt_pulsewriter_t *p)
{
	pa_sample_spec ss;

	ss.format = PA_SAMPLE_S16NE;
	ss.channels = 1;
	ss.rate = p->ctx->sampling_frequency;

	p->pulse = pa_simple_new(NULL,             // Use the default server.
			"Tintamp",          // Our application's name.
			PA_STREAM_PLAYBACK,
			NULL,               // Use the default device.
			"Music",            // Description of our stream.
			&ss,                // Our sample format.
			NULL,               // Use default channel map
			NULL,               // Use default buffering attributes.
			NULL                // Ignore error code.
	);
}

void tt_pulsewriter_step(tt_pulsewriter_t *p, ttspl_t spl)
{
	int16_t s16 = TTASS16LE(spl);

	(void) pa_simple_write(p->pulse, &s16, sizeof(s16), NULL);
}

void tt_pulsewriter_process(tt_pulsewriter_t *p, tt_sbuf_t *outbuf)
{
	int sz = p->ctx->grain_size * sizeof(int16_t);
	int16_t *buf = alloca(sz);
	tt_sbuf_to_s16le(outbuf, buf, p->ctx->grain_size);
	(void) pa_simple_write(p->pulse, buf, sz, NULL);
}

