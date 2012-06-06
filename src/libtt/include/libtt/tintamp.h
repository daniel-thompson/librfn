/*
 * tintamp.h
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

#ifndef TT_TINTAMP_H_
#define TT_TINTAMP_H_

#include "util.h"
#include "cabsim.h"

typedef struct {
	tt_cabsim_t cabsim;
} tt_tintamp_t;

void tt_tintamp_init(tt_tintamp_t *tt, tt_context_t *ctx);
void tt_tintamp_finalize(tt_tintamp_t *tt);
tt_tintamp_t *tt_tintamp_new(tt_context_t *ctx);
void tt_tintamp_delete(tt_tintamp_t *tt);

// no step (tintamp has/will have) stereo output)
void tt_tintamp_process(tt_tintamp_t *tt, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_TINTAMP_H_
