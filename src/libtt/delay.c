/*
 * delay.c
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

#include <string.h>

#include "libtt.h"
#include "libtt/delay.h"

void tt_delay_init(tt_delay_t *p, tt_context_t *ctx, ttspl_t *buf, unsigned int len)
{
	memset(p, 0, sizeof(*p));
	p->ctx = ctx;

	p->buf = buf;
	p->buflen = len;

	// the delay is set using the _set_control() interface (meaning
	// the ceiling is fairly low)
	assert(len < TTSPL_MAX);
	p->controls[TT_TAG2ID(TT_DELAY_CONTROL_NUM_SAMPLES)] = TTINT(len);
}

void tt_delay_finalize(tt_delay_t *p)
{
}

tt_delay_t *tt_delay_new(tt_context_t *ctx, unsigned int len)
{
	char *m = xmalloc(sizeof(tt_delay_t) + (sizeof(ttspl_t)*len));
	tt_delay_t *p = (void *) m;
	ttspl_t *buf = (void *) (m + sizeof(tt_delay_t));
	tt_delay_init(p, ctx, buf, len);
	return p;
}

tt_generic_delete(delay);

void tt_delay_setup(tt_delay_t *p)
{

	ttspl_t delay = p->controls[TT_TAG2ID(TT_DELAY_CONTROL_NUM_SAMPLES)];

	// ensure the delay is not out of range
	if (TTASINT(delay) > p->buflen) {
		delay = p->buflen;
		p->controls[TT_TAG2ID(TT_DELAY_CONTROL_NUM_SAMPLES)] = TTINT(delay);
	}

	// calculate the new buffer end
	ttspl_t *bufend = p->buf + TTASINT(delay);

	// lazily update the filter state
	if (p->bufend != bufend) {
		memset(p->buf, 0, TTASINT(delay) * sizeof(ttspl_t));

		p->p = p->buf;
		p->bufend = bufend;
	}
}

tt_generic_get_control(delay, TT_DELAY_CONTROL_MIN, TT_DELAY_CONTROL_MAX);

void tt_delay_set_control(tt_delay_t *p, tt_delay_control_t ctrl, ttspl_t val)
{
	assert(ctrl >= TT_DELAY_CONTROL_MIN && ctrl < TT_DELAY_CONTROL_MAX);
	p->controls[TT_TAG2ID(ctrl)] = val;

	// apply the change
	tt_delay_setup(p);
}

tt_generic_enum_control(delay, TT_DELAY_CONTROL_MIN, TT_DELAY_CONTROL_MAX);


/*!
 * \todo Does not to fractional delay...
 */
ttspl_t tt_delay_step(tt_delay_t *p, ttspl_t spl)
{
	ttspl_t newspl = *(p->p);
	*(p->p) = spl;

	if (++(p->p) >= p->bufend)
		p->p = p->buf;

	return newspl;
}

tt_generic_process(delay);
