/*
 * preset.c
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

#include <string.h>

#include "libtt.h"

const tt_preset_ops_t tt_preset_ops_drummachine = TT_PRESET_OPS_INITIALIZER(
		tt_drummachine_get_control,
		tt_drummachine_set_control,
		tt_drummachine_enum_control
);

const tt_preset_ops_t tt_preset_ops_tintamp = TT_PRESET_OPS_INITIALIZER(
		tt_tintamp_get_control,
		tt_tintamp_set_control,
		tt_tintamp_enum_control
);

const tt_preset_ops_t tt_preset_ops_tintdrum = TT_PRESET_OPS_INITIALIZER(
		tt_tintdrum_get_control,
		tt_tintdrum_set_control,
		tt_tintdrum_enum_control
);

void tt_preset_init(tt_preset_t *p, const tt_preset_ops_t *ops)
{
	memset(p, 0, sizeof(*p));
	p->ops = ops;
	tt_preset_clear(p);
}

void tt_preset_finalize(tt_preset_t *p)
{
	// do nothing
}

tt_preset_t * tt_preset_new(const tt_preset_ops_t *ops)
{
	tt_preset_t *p = malloc(sizeof(tt_preset_t));
	if (p)
		tt_preset_init(p, ops);
	return p;
}

void tt_preset_delete(tt_preset_t *p)
{
	tt_preset_finalize(p);
	free(p);
}

void tt_preset_clear(tt_preset_t *p)
{
	for (int i=0; i<lengthof(p->controls); i++) {
		p->controls[i].ctrl = -1;
		p->controls[i].val = TTINT(0);
	}
}

void tt_preset_save(tt_preset_t *p, void *q)
{
	tt_preset_clear(p);

	for (int ctrl = p->ops->enum_control(0);
	     ctrl != 0;
	     ctrl = p->ops->enum_control(ctrl))
		tt_preset_deserialize(p, ctrl, p->ops->get_control(q, ctrl));
}

void tt_preset_restore(tt_preset_t *p, void *q)
{
	tt_preset_serialize(p, q, p->ops->set_control);
}

void tt_preset_serialize(tt_preset_t *p, void *q, tt_preset_set_control_t *serialize)
{
	for (int i=0; p->controls[i].ctrl != -1; i++)
		serialize(q, p->controls[i].ctrl, p->controls[i].val);
}

void tt_preset_deserialize(tt_preset_t *p, int ctrl, ttspl_t val)
{
	int i;

	for (i=0; p->controls[i].ctrl != -1; i++)
		assert(i < lengthof(p->controls));

	p->controls[i].ctrl = ctrl;
	p->controls[i].val = val;
}
