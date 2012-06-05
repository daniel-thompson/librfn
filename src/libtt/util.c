/*
 * util.c
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

#include "librfn.h"
#include "libtt.h"

void tt_context_init(tt_context_t *ctx)
{
	ctx->grain_size = 256;
	ctx->sampling_frequency = 48000;
}

tt_context_t *tt_context_new()
{
	tt_context_t *p = xmalloc(sizeof(*p));
	tt_context_init(p);
	return p;
}

void tt_context_delete(tt_context_t *ctx)
{
	free(ctx);
}
