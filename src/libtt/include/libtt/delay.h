/*
 * delay.h
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

#ifndef TT_DELAY_H_
#define TT_DELAY_H_

#include "libtt/sample.h"
#include "libtt/util.h"

typedef enum {
	TT_DELAY_CONTROL_MIN = TT_BASE2TAG(TT_DELAY_BASE),
	TT_DELAY_CONTROL_NUM_SAMPLES = TT_DELAY_CONTROL_MIN,
	TT_DELAY_CONTROL_MAX
} tt_delay_control_t;

typedef struct {
	tt_context_t *ctx;

	ttspl_t controls[TT_TAG2ID(TT_DELAY_CONTROL_MAX)];

	ttspl_t *buf;
	unsigned int buflen;
	ttspl_t *bufend;

	ttspl_t *p;
} tt_delay_t;

void tt_delay_init(tt_delay_t *p, tt_context_t *ctx, ttspl_t *buf, unsigned int len);
void tt_delay_finalize(tt_delay_t *p);
tt_delay_t *tt_delay_new(tt_context_t *ctx, unsigned int len);
void tt_delay_delete(tt_delay_t *p);

void tt_delay_setup(tt_delay_t *p);
ttspl_t tt_delay_get_control(tt_delay_t *p, tt_delay_control_t ctrl);
void tt_delay_set_control(tt_delay_t *p, tt_delay_control_t ctrl, ttspl_t val);
tt_delay_control_t tt_delay_enum_control(tt_delay_control_t ctrl);

static inline ttspl_t tt_delay_peek(tt_delay_t *p)
{
	return *(p->p);
}

ttspl_t tt_delay_step(tt_delay_t *p, ttspl_t spl);
void tt_delay_process(tt_delay_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_DELAY_H_
