/*
 * quantizer.h
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

#ifndef TT_QUANTIZER_H_
#define TT_QUANTIZER_H_

#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

	unsigned int shift;

} tt_quantizer_t;

void tt_quantizer_init(tt_quantizer_t *q, tt_context_t *ctx);
void tt_quantizer_finalize(tt_quantizer_t *q);
tt_quantizer_t *tt_quantizer_new(tt_context_t *ctx);
void tt_quantizer_delete(tt_quantizer_t *q);

void tt_quantizer_setup(tt_quantizer_t *q, unsigned int nbits);

ttspl_t tt_quantizer_step(tt_quantizer_t *q, ttspl_t spl);
void tt_quantizer_process(tt_quantizer_t *q, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_QUANTIZER_H_
