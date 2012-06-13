/*
 * wavwriter.h
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

#ifndef TT_WAVWRITER_H_
#define TT_WAVWRITER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "librfn/wavheader.h"
#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

	FILE *file;

	rf_wavheader_t wavheader;
	unsigned int num_frames;
} tt_wavwriter_t;

void tt_wavwriter_init(tt_wavwriter_t *ww, tt_context_t *ctx);
void tt_wavwriter_finalize(tt_wavwriter_t *ww);
tt_wavwriter_t *tt_wavwriter_new(tt_context_t *ctx);
void tt_wavwriter_delete(tt_wavwriter_t *ww);

int tt_wavwriter_setup(tt_wavwriter_t *ww, const char *fname,
		rf_wavheader_format_t format);

void tt_wavwriter_step(tt_wavwriter_t *ww, ttspl_t spl);
void tt_wavwriter_process(tt_wavwriter_t *ww, tt_sbuf_t *outbuf);

#endif // TT_WAVWRITER_H_
