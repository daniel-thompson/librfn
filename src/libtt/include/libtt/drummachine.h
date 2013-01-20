/*
 * drummachine.h
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

#ifndef TT_DRUMMACHINE_H_
#define TT_DRUMMACHINE_H_

#include "libtt/sample.h"
#include "libtt/util.h"

typedef enum {
	TT_DRUMMACHINE_CONTROL_MIN = TT_BASE2TAG(TT_DRUMMACHINE_BASE),
	TT_DRUMMACHINE_CONTROL_BPM = TT_DRUMMACHINE_CONTROL_MIN,
	TT_DRUMMACHINE_CONTROL_PATTERN,
	TT_DRUMMACHINE_CONTROL_PATTERN_MAX,
	TT_DRUMMACHINE_CONTROL_MAX
} tt_drummachine_control_t;

typedef struct {
	tt_context_t *ctx;

	ttspl_t controls[TT_TAG2ID(TT_DRUMMACHINE_CONTROL_MAX)];

	bool cold_sample; //!< True for "cold" samples (0 into the resampler)
	tt_biquad_t resampler;

	int division_counter;
	int division_reload;

	int voice_pointer[8];

	uint8_t *pattern_start;
	uint8_t *pattern_end;
	uint8_t *pattern_pointer;
} tt_drummachine_t;

void tt_drummachine_init(tt_drummachine_t *dm, tt_context_t *ctx);
void tt_drummachine_finalize(tt_drummachine_t *dm);
tt_drummachine_t *tt_drummachine_new(tt_context_t *ctx);
void tt_drummachine_delete(tt_drummachine_t *dm);

void tt_drummachine_setup(tt_drummachine_t *dm);
ttspl_t tt_drummachine_get_control(tt_drummachine_t *dm, tt_drummachine_control_t ctrl);
void tt_drummachine_set_control(tt_drummachine_t *dm, tt_drummachine_control_t ctrl, ttspl_t val);
tt_drummachine_control_t tt_drummachine_enum_control(tt_drummachine_control_t ctrl);

unsigned int tt_drummachine_samples_until_next_beat(tt_drummachine_t *dm);

ttspl_t tt_drummachine_step(tt_drummachine_t *dm);
void tt_drummachine_process(tt_drummachine_t *dm, tt_sbuf_t *outbuf);

#endif // TT_DRUMMACHINE_H_
