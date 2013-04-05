/*
 * preset.h
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

#ifndef TT_PRESET_H_
#define TT_PRESET_H_

#include "libtt/sample.h"
#include "libtt/util.h"

#define TT_PRESET_NUM_CONTROLS 16

typedef ttspl_t tt_preset_get_control_t(void *q, int ctrl);
typedef void tt_preset_set_control_t(void *q, int ctrl, ttspl_t val);
typedef int tt_preset_enum_control_t(int ctrl);

typedef struct {
	tt_preset_get_control_t *get_control;
	tt_preset_set_control_t *set_control;
	tt_preset_enum_control_t *enum_control;
} tt_preset_ops_t;

#define TT_PRESET_OPS_INITIALIZER(get, set, enumerate) \
{ \
	(tt_preset_get_control_t *) get, \
	(tt_preset_set_control_t *) set, \
	(tt_preset_enum_control_t *) enumerate \
}

extern const tt_preset_ops_t tt_preset_ops_drummachine;
extern const tt_preset_ops_t tt_preset_ops_tintamp;
extern const tt_preset_ops_t tt_preset_ops_tintdrum;

typedef struct {
	const tt_preset_ops_t *ops;
	struct {
		int ctrl;
		ttspl_t val;
	} controls[TT_PRESET_NUM_CONTROLS];
} tt_preset_t;

void tt_preset_init(tt_preset_t *p, const tt_preset_ops_t *ops);
void tt_preset_finalize(tt_preset_t *p);
tt_preset_t *tt_preset_new(const tt_preset_ops_t *ops);
void tt_preset_delete(tt_preset_t *p);

void tt_preset_clear(tt_preset_t *p);
void tt_preset_save(tt_preset_t *p, void *q);
void tt_preset_restore(tt_preset_t *p, void *q);

void tt_preset_serialize(tt_preset_t *p, void *q, tt_preset_set_control_t *serialize);
void tt_preset_deserialize(tt_preset_t *p, int ctrl, ttspl_t val);

#endif // TT_PRESET_H_
