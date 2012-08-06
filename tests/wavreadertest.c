/*
 * wavreadertest.c
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

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <librfn.h>
#include <libtt.h>

int main(int argc, char **argv)
{
	int cycle_limit = 200;
	char *fname = "wavreadertest.wav";

	if (argc >= 2) {
		fname = argv[1];
		cycle_limit = 0;
	}

	// INIT
	tt_context_t *ctx = tt_context_new();
	tt_wavreader_t *wr = tt_wavreader_new(ctx);

	// update the context from the file we're reading from
	//ctx->sampling_frequency = wr->wavheader.sample_rate;

	tt_sbuf_t *inbuf = tt_sbuf_new(ctx->grain_size);
	tt_sbuf_t *outbuf = tt_sbuf_new(ctx->grain_size);
	tt_wavwriter_t *ww = tt_wavwriter_new(ctx);
	tt_quantizer_t *q = tt_quantizer_new(ctx);
	tt_tintamp_t *tt = tt_tintamp_new(ctx);

	int res = tt_wavreader_setup(wr, fname);
	assert(0 == res);

	res = tt_wavwriter_setup(ww, "wavwritertest.wav", RF_WAVHEADER_S32LE);
	assert(0 == res);
	char *desc = rf_wavheader_tostring(&wr->wavheader);
	printf("%s\n", desc);
	free(desc);

	tt_quantizer_setup(q, 14);

	int i;
	for (i=0; wr->file && (cycle_limit==0 || i<cycle_limit); i++) {
		tt_wavreader_process(wr, outbuf);
		tt_quantizer_process(q, outbuf, inbuf);
		tt_tintamp_process(tt, inbuf, outbuf);
		tt_wavwriter_process(ww, outbuf);
	}
	printf("Completed after %d loops\n", i);
	assert(cycle_limit == 0 || i == cycle_limit);

	// TIDY
	tt_tintamp_delete(tt);
	tt_quantizer_delete(q);
	tt_wavwriter_delete(ww);
	tt_wavreader_delete(wr);
	tt_sbuf_delete(outbuf);
	tt_sbuf_delete(inbuf);
	tt_context_delete(ctx);

	return 0;
}
