/*
 * presetio.h
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

#ifndef TT_PRESETIO_H_
#define TT_PRESETIO_H_

#include <stdio.h>

#include "librfn/enum.h"
#include "libtt/preset.h"

extern rf_enumtable_t tt_presetio_enumtable[];

void tt_presetio_serializer(void *p, int ctrl, ttspl_t val);

void tt_presetio_serialize(FILE *f, tt_preset_t *pre);
void tt_presetio_deserialize(FILE *f, tt_preset_t *pre);

#endif // TT_PRESETIO_H_
