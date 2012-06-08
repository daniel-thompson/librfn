/*
 * preamp.h
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

#ifndef TT_PREAMP_H_
#define TT_PREAMP_H_

#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

	unsigned int num_stages;
	tt_tubestage_t stages[3];

	tt_sbuf_t *tmpbuf;
} tt_preamp_t;

typedef enum {
	TT_PREAMP_CLEAN
} tt_preamp_model_t;

void tt_preamp_init(tt_preamp_t *p, tt_context_t *ctx);
void tt_preamp_finalize(tt_preamp_t *p);
tt_preamp_t *tt_preamp_new(tt_context_t *ctx);
void tt_preamp_delete(tt_preamp_t *p);

void tt_preamp_setup(tt_preamp_t *p, tt_preamp_model_t model);

ttspl_t tt_preamp_step(tt_preamp_t *p, ttspl_t spl);
void tt_preamp_process(tt_preamp_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_PREAMP_H_
