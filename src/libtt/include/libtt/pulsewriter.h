/*
 * pulsewriter.h
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

#ifndef TT_PULSEWRITER_H_
#define TT_PULSEWRITER_H_

#include <pulse/simple.h>

#include "libtt/sample.h"
#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

	pa_simple *pulse;
} tt_pulsewriter_t;

void tt_pulsewriter_init(tt_pulsewriter_t *p, tt_context_t *ctx);
void tt_pulsewriter_finalize(tt_pulsewriter_t *p);
tt_pulsewriter_t *tt_pulsewriter_new(tt_context_t *ctx);
void tt_pulsewriter_delete(tt_pulsewriter_t *p);

void tt_pulsewriter_setup(tt_pulsewriter_t *p);

void tt_pulsewriter_step(tt_pulsewriter_t *p, ttspl_t spl);
void tt_pulsewriter_process(tt_pulsewriter_t *p, tt_sbuf_t *inbuf);

#endif // TT_PULSEWRITER_H_
