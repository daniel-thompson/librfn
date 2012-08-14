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

typedef enum {
	TT_PREAMP_CONTROL_MIN = TT_BASE2TAG(TT_PREAMP_BASE),
	TT_PREAMP_CONTROL_MODEL = TT_PREAMP_CONTROL_MIN,
	TT_PREAMP_CONTROL_GAIN,
	TT_PREAMP_CONTROL_MAX
} tt_preamp_control_t;

typedef enum {
	TT_PREAMP_CLEAN
} tt_preamp_model_t;

typedef struct {
	tt_context_t *ctx;

	ttspl_t controls[TT_PREAMP_CONTROL_MAX];

	unsigned int num_stages;
	tt_tubestage_t stages[3];

	tt_sbuf_t *tmpbuf;
} tt_preamp_t;

void tt_preamp_init(tt_preamp_t *p, tt_context_t *ctx);
void tt_preamp_finalize(tt_preamp_t *p);
tt_preamp_t *tt_preamp_new(tt_context_t *ctx);
void tt_preamp_delete(tt_preamp_t *p);

void tt_preamp_setup(tt_preamp_t *p, tt_preamp_model_t model);
ttspl_t tt_preamp_get_control(tt_preamp_t *p, tt_preamp_control_t ctrl);
void tt_preamp_set_control(tt_preamp_t *p, tt_preamp_control_t ctrl, ttspl_t val);
tt_preamp_control_t tt_preamp_enum_control(tt_preamp_control_t ctrl);

ttspl_t tt_preamp_step(tt_preamp_t *p, ttspl_t spl);
void tt_preamp_process(tt_preamp_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_PREAMP_H_
