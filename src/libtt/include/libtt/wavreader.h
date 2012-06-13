/*
 * wavreader.h
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

#ifndef TT_WAVREADER_H_
#define TT_WAVREADER_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "librfn/wavheader.h"
#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

	FILE *file;

	rf_wavheader_t wavheader;
	rf_wavheader_format_t sample_type;

	bool endian_swap;
} tt_wavreader_t;

void tt_wavreader_init(tt_wavreader_t *wr, tt_context_t *ctx);
void tt_wavreader_finalize(tt_wavreader_t *wr);
tt_wavreader_t *tt_wavreader_new(tt_context_t *ctx);
void tt_wavreader_delete(tt_wavreader_t *wr);

int tt_wavreader_setup(tt_wavreader_t *wr, const char *fname);

ttspl_t tt_wavreader_step(tt_wavreader_t *wr);
void tt_wavreader_process(tt_wavreader_t *wr, tt_sbuf_t *outbuf);

#endif // TT_WAVREADER_H_
