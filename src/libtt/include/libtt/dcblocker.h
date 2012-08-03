/*
 * dcblocker.h
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

#ifndef TT_DCBLOCKER_H_
#define TT_DCBLOCKER_H_

#include <stdbool.h>

#include "firstorder.h"
#include "sbuf.h"
#include "util.h"

typedef tt_firstorder_t tt_dcblocker_t;

void tt_dcblocker_init(tt_dcblocker_t *dc, tt_context_t *ctx);
static inline void tt_dcblocker_finalize(tt_dcblocker_t *dc) {}
tt_dcblocker_t *tt_dcblocker_new(tt_context_t *ctx);
void tt_dcblocker_delete(tt_dcblocker_t *dc);

ttspl_t tt_dcblocker_step(tt_dcblocker_t *dc, ttspl_t spl);
void tt_dcblocker_process(tt_dcblocker_t *dc, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);
void tt_dcblocker_flush(tt_dcblocker_t *dc);

void tt_dcblocker_setup(tt_dcblocker_t *dc, int freq);

#endif // TT_DCBLOCKER_H_
