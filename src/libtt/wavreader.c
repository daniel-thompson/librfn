/*
 * wavreader.c
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

void tt_wavreader_init(tt_wavreader_t *wr, tt_context_t *ctx)
{
	memset(wr, 0, sizeof(*wr));
	wr->ctx = ctx;
}

void tt_wavreader_finalize(tt_wavreader_t *wr)
{
	if (wr->file) {
		fclose(wr->file);
		wr->file = NULL;
	}
}

tt_generic_new(wavreader);
tt_generic_delete(wavreader);

int tt_wavreader_setup(tt_wavreader_t *wr, const char *fname)
{
	if (wr->file)
		tt_wavreader_finalize(wr);

	wr->file = fopen(fname, "rb");
	if (!wr->file)
		return -1;

	uint8_t header[128];
	int header_size = 0;
	int needed = RF_WAVHEADER_MIN_SIZE;
	rf_wavheader_t wavheader;

	while (needed > header_size) {
		int res = fread(header+header_size, needed-header_size, 1, wr->file);
		if (1 != res)
			goto failed;
		header_size = needed;

		needed = rf_wavheader_decode(header, header_size, &wavheader);
		if (needed < header_size || needed > sizeof(header))
			goto failed;
	}

	wr->wavheader = wavheader;
	wr->sample_type = rf_wavheader_get_format(&wavheader);
	if (RF_WAVHEADER_UNKNOWN == wr->sample_type)
		goto failed;
	wr->endian_swap = false;

	return 0;

    failed:
    	tt_wavreader_finalize(wr);
    	return -1;
}

inline ttspl_t tt_wavreader_step(tt_wavreader_t *wr)
{
	union {
		uint8_t raw[32];
		int16_t s16[16];
		int32_t s32[8];
		float f[8];
	} frame;

	unsigned int sample_stride = wr->wavheader.block_align;
	unsigned int sample_size = wr->wavheader.bits_per_sample / 8;

	ttspl_t spl = TTINT(0);
	assert(sample_stride <= sizeof(frame.raw));

	// return zero if we have no file
	if (NULL == wr->file)
		return TTINT(0);


	// extract a frame from the file
	int res = fread(frame.raw, 1, sample_stride, wr->file);

	// extract a sample if we've read enough data
	if (res >= sample_size) {
		if (wr->endian_swap) {
			for (unsigned int i=0; i<sample_size/2; i++) {
				unsigned int j = sample_size-1-i;
				uint8_t ss = frame.raw[i];
				frame.raw[i] = frame.raw[j];
				frame.raw[j] = ss;
			}
		}

		switch (wr->sample_type) {
		case RF_WAVHEADER_S16LE:
			spl = TTS16LE(frame.s16[0]);
			break;
		case RF_WAVHEADER_S32LE:
			assert(0); // not implemented
			spl = TTINT(0);
			break;
		case RF_WAVHEADER_FLOAT:
			spl = TTFLOAT(frame.f[0]);
			break;
		default:
			assert(RF_WAVHEADER_UNKNOWN == wr->sample_type);
			assert(0);
			break;
		}
	}

	// close down if we've not read a whole sample
	if (res < sample_stride)
		tt_wavreader_finalize(wr);

	return spl;
}

tt_generic_output(wavreader);
