/*
 * presetio.c
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

// use controls.h to ensure the preset control array is large enough
#ifndef NDEBUG
tt_preset_t tt_presetio_overflow_check = {
		NULL,
		{
#define CONTROL(x) { x, TTINT(0) }
#include "controls.h"
#undef CONTROL
		}
};
#endif

// use controls.h to populate the control enum lookup table
rf_enumtable_t tt_presetio_enumtable[] = {
#define CONTROL(x) { #x, x }
#include "controls.h"
#undef CONTROL
RF_ENUMTABLE_TERMINATOR
};

void tt_presetio_serializer(void *p, int ctrl, ttspl_t val)
{
	FILE *f = p;
	const char *sctrl = rf_enum2string(tt_presetio_enumtable, ctrl);;
#ifdef HAVE_FPU
	int32_t ival = ((int32_t) ((val) * (float) (1 << TTQ)));
#else
	int32_t ival = val;
#endif

	fprintf(f, "%-35s : 0x%08x\n", sctrl, ival);
}

void tt_presetio_serialize(FILE *f, tt_preset_t *pre)
{
	tt_preset_serialize(pre, f, tt_presetio_serializer);
}

void tt_presetio_deserialize(FILE *f, tt_preset_t *pre)
{
	char line[80];
	int32_t ival;
	int ctrl;
	ttspl_t val;

	tt_preset_clear(pre);

	while (NULL != fgets(line, sizeof(line), f)) {
		char *p = strchr(line, ' ');
		if (!p)
			continue;
		*p++ = '\0';

		ctrl = rf_string2enum(tt_presetio_enumtable, line);
		if (ctrl == RF_ENUM_OUT_OF_RANGE)
			continue;

		p = strchr(p, ':');
		if (!p)
			continue;
		p++;
		ival = strtol(p, NULL, 0);

#ifdef HAVE_FPU
		val = ((float) (ival) / (float) (1 << TTQ));
#else
		val = ival;
#endif

		tt_preset_deserialize(pre, ctrl, val);
	}
}
