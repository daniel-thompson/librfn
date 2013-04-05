/*
 * tintdrum.h
 *
 * Part of libtt (the integer amplifier library)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef TT_TINTDRUM_H_
#define TT_TINTDRUM_H_

#include "util.h"
#include "drummachine.h"
#include "reverb.h"

typedef enum {
	TT_TINTDRUM_CONTROL_MIN = TT_BASE2TAG(TT_TINTDRUM_BASE),
	TT_TINTDRUM_CONTROL_MASTER_VOLUME = TT_TINTDRUM_CONTROL_MIN,
	TT_TINTDRUM_CONTROL_MAX
} tt_tintdrum_control_t;

typedef struct {
	tt_context_t *ctx;

	tt_drummachine_t drummachine;
	tt_reverb_t reverb;

	tt_sbuf_t *tmpbuf;
} tt_tintdrum_t;

void tt_tintdrum_init(tt_tintdrum_t *tt, tt_context_t *ctx);
void tt_tintdrum_finalize(tt_tintdrum_t *tt);
tt_tintdrum_t *tt_tintdrum_new(tt_context_t *ctx);
void tt_tintdrum_delete(tt_tintdrum_t *tt);

ttspl_t tt_tintdrum_get_control(tt_tintdrum_t *tt, tt_tintdrum_control_t ctrl);
void tt_tintdrum_set_control(tt_tintdrum_t *tt, tt_tintdrum_control_t ctrl, ttspl_t val);
tt_tintdrum_control_t tt_tintdrum_enum_control(tt_tintdrum_control_t ctrl);

// no step (tintdrum has/will have) stereo output)
void tt_tintdrum_process(tt_tintdrum_t *tt, tt_sbuf_t *outbuf);

#endif // TT_TINTDRUM_H_
