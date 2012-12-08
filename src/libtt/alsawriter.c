/*
 * alsawriter.c
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
#include "libtt/alsawriter.h"


void tt_alsawriter_init(tt_alsawriter_t *p, tt_context_t *ctx)
{
	memset(p, 0, sizeof(*p));
	p->ctx = ctx;
}

void tt_alsawriter_finalize(tt_alsawriter_t *p)
{
	if (p->handle) {
		snd_pcm_close(p->handle);
	}
}

tt_generic_new(alsawriter);
tt_generic_delete(alsawriter);

void tt_alsawriter_setup(tt_alsawriter_t *p)
{
	int err;

	err = snd_pcm_open(&p->handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if (0 != err) {
		// do something...
	}

	err = snd_pcm_set_params(p->handle,
			SND_PCM_FORMAT_S16,
			SND_PCM_ACCESS_RW_INTERLEAVED,
			1,	// mono
			p->ctx->sampling_frequency,
			1,	// allow s/ware resampling
			100000); // 0.1 seconds
	if (0 == err) {
		// do something...
	}
}

void tt_alsawriter_step(tt_alsawriter_t *p, ttspl_t spl)
{
	int16_t s16 = TTASS16LE(spl);

        snd_pcm_sframes_t res;

        do {
        	res = snd_pcm_writei(p->handle, &s16, 1);
        	if (res < 0)
        		(void) snd_pcm_recover(p->handle, res, 0);
        } while (res < 0);
}

void tt_alsawriter_process(tt_alsawriter_t *p, tt_sbuf_t *outbuf)
{
	int sz = p->ctx->grain_size * sizeof(int16_t);
	int16_t *buf = alloca(sz);
	snd_pcm_sframes_t remaining = p->ctx->grain_size;
	snd_pcm_sframes_t res;

	tt_sbuf_to_s16le(outbuf, buf, p->ctx->grain_size);

	do {
		res = snd_pcm_writei(p->handle, buf, remaining);
		if (res < 0)
			(void) snd_pcm_recover(p->handle, res, 0);
		else
			buf += res, remaining -= res;
	} while (remaining > 0);
}

