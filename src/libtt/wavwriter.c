/*
 * wavwriter.c
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

#include <assert.h>
#include <string.h>

#include "librfn.h"
#include "libtt.h"

void tt_wavwriter_init(tt_wavwriter_t *ww, tt_context_t *ctx)
{
	memset(ww, 0, sizeof(*ww));
	ww->ctx = ctx;
}

void tt_wavwriter_finalize(tt_wavwriter_t *ww)
{
	if (ww->file) {
		// try to update the wavheader
		if (0 == fseek(ww->file, 0, SEEK_SET)) {
			uint8_t header[128];
			rf_wavheader_set_num_frames(&ww->wavheader, ww->num_frames);

			unsigned int sz = rf_wavheader_encode(
					&ww->wavheader, header, sizeof(header));
			assert(sz < sizeof(header));

			(void) fwrite(header, sz, 1, ww->file);
		}

		// whatever happens just close the file
		fclose(ww->file);
		ww->file = NULL;
	}
}

tt_generic_new(wavwriter);
tt_generic_delete(wavwriter);

int tt_wavwriter_setup(tt_wavwriter_t *ww, const char *fname,
		rf_wavheader_format_t format)
{
	if (ww->file)
		tt_wavwriter_finalize(ww);

	ww->file = fopen(fname, "wb");
	if (!ww->file)
		return -1;

	rf_wavheader_init(&ww->wavheader, ww->ctx->sampling_frequency,
			1, format);

	uint8_t header[128];
	unsigned int sz = rf_wavheader_encode(
				&ww->wavheader, header, sizeof(header));
	assert(sz < sizeof(header));

	if (1 != fwrite(header, sz, 1, ww->file))
		tt_wavwriter_finalize(ww);

	return 0;
}

inline void tt_wavwriter_step(tt_wavwriter_t *ww, ttspl_t spl)
{
	union {
		uint8_t raw[32];
		int16_t s16[16];
		int32_t s32[8];
		float f[8];
	} frame;

	// TODO: Currently we are hardcoded for mono floating point
	frame.s32[0] = TTASS32LE(spl);

	int res = fwrite(frame.raw, sizeof(uint32_t), 1, ww->file);
	if (1 == res)
		ww->num_frames++;
	else
		tt_wavwriter_finalize(ww);
}

tt_generic_input(wavwriter);
