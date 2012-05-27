/*
 * cabsim.h
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

#ifndef TT_CABSIM_H_
#define TT_CABSIM_H_

#include "biquad.h"

typedef struct {
	tt_biquad_t lpf0;
	tt_biquad_t lpf1;
} tt_cabsim_t;

tt_cabsim_t *tt_cabsim_new();
void tt_cabsim_delete(tt_cabsim_t *cabsim);

void tt_cabsim_process(tt_cabsim_t *cabsim, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

void tt_cabsim_tune(tt_cabsim_t *cabsim);

#endif // CABSIM_H_
