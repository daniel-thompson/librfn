/*
 * template.c
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
#include "libtt/template.h"

void tt_template_init(tt_template_t *p, tt_context_t *ctx)
{
	memset(p, 0, sizeof(*p));
	p->ctx = ctx;
}

void tt_template_finalize(tt_template_t *p)
{
}

tt_generic_new(template);
tt_generic_delete(template);

void tt_template_setup(tt_template_t *p)
{
}

tt_generic_get_control(template, TT_TEMPLATE_CONTROL_MIN, TT_TEMPLATE_CONTROL_MAX);

void tt_template_set_control(tt_template_t *p, tt_template_control_t ctrl, ttspl_t val)
{
	assert(ctrl >= TT_TEMPLATE_CONTROL_MIN && ctrl < TT_TEMPLATE_CONTROL_MAX);
	p->controls[TT_TAG2ID(ctrl)] = val;

	// apply the change
}

tt_generic_enum_control(template, TT_TEMPLATE_CONTROL_MIN, TT_TEMPLATE_CONTROL_MAX);


ttspl_t tt_template_step(tt_template_t *p, ttspl_t spl)
{
	return spl;
}

void tt_template_process(tt_template_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
}
