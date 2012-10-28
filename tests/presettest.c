/*
 * presettest.c
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

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <librfn.h>
#include <libtt.h>

void serialize_and_increment(void *p, int ctrl, ttspl_t val)
{
	tt_tintamp_t *tt = p;

	tt_tintamp_set_control(tt, ctrl, TTADD(val, TTINT(1)));
}

void serialize_and_count(void *p, int ctrl, ttspl_t val)
{
	int *ip = p;
	(*ip)++;
}

int main()
{
	// INIT
	tt_context_t *ctx = tt_context_new();
	ctx->sampling_frequency = 48000;
	tt_tintamp_t *tt = tt_tintamp_new(ctx);
	tt_preset_t *pre = tt_preset_new(&tt_preset_ops_tintamp);
	tt_preset_t *pre2 = tt_preset_new(&tt_preset_ops_tintamp);

	// save a preset, print it ('cos it looks nice) and add one to each control
	tt_preset_save(pre, tt);
	tt_presetio_serialize(stdout, pre);
	tt_preset_serialize(pre, tt, serialize_and_increment);

	// save a second preset and verify it differs from the original one
	tt_preset_save(pre2, tt);
	assert(0 != memcmp(pre, pre2, sizeof(*pre)));

	// restore the preset and save it again, now it must be the same
	tt_preset_restore(pre, tt);
	tt_preset_save(pre2, tt);
	assert(0 == memcmp(pre, pre2, sizeof(*pre)));

	// clear it and it must be different
	tt_preset_clear(pre2);
	assert(0 != memcmp(pre, pre2, sizeof(*pre)));

	// check that we serialize the right number of items
	int i = 0, j = 0;
	tt_preset_serialize(pre, &i, serialize_and_count);
	for (int k=tt_tintamp_enum_control(0); k!=0; k=tt_tintamp_enum_control(k))
		j++;
	assert(i == j);

	// serialize to file, deserialize and compare
	//
	// Note: this is a bit-exact compare so this will only pass in FPU
	//       mode if the controls themselves have easily represented values
	FILE *f;
	f = fopen("presettest.pre", "w");
	tt_presetio_serialize(f, pre);
	fclose(f);
	f = fopen("presettest.pre", "r");
	tt_presetio_deserialize(f, pre2);
	fclose(f);
	assert(0 == memcmp(pre, pre2, sizeof(*pre)));

	// TIDY
	tt_preset_delete(pre2);
	tt_preset_delete(pre);
	tt_tintamp_delete(tt);
	tt_context_delete(ctx);

	return 0;
}
