/*
 * alsawriter.h
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

#ifndef TT_ALSAWRITER_H_
#define TT_ALSAWRITER_H_

#include <alsa/asoundlib.h>

#include "libtt/sample.h"
#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

	snd_pcm_t *handle;
} tt_alsawriter_t;

void tt_alsawriter_init(tt_alsawriter_t *p, tt_context_t *ctx);
void tt_alsawriter_finalize(tt_alsawriter_t *p);
tt_alsawriter_t *tt_alsawriter_new(tt_context_t *ctx);
void tt_alsawriter_delete(tt_alsawriter_t *p);

void tt_alsawriter_setup(tt_alsawriter_t *p);

void tt_alsawriter_step(tt_alsawriter_t *p, ttspl_t spl);
void tt_alsawriter_process(tt_alsawriter_t *p, tt_sbuf_t *inbuf);

#endif // TT_ALSAWRITER_H_
