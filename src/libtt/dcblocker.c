/*
 * dcblocker.c
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "librfn.h"
#include "libtt.h"

void tt_dcblocker_init(tt_dcblocker_t *dc, tt_context_t *ctx)
{
	tt_firstorder_init(dc, ctx);
}

tt_dcblocker_t *tt_dcblocker_new(tt_context_t *ctx)
{
	return tt_firstorder_new(ctx);
}

void tt_dcblocker_delete(tt_dcblocker_t *dc)
{
	tt_firstorder_delete(dc);
}

ttspl_t tt_dcblocker_step(tt_dcblocker_t *dc, ttspl_t spl)
{
	return tt_firstorder_step(dc, spl);
}

void tt_dcblocker_process(tt_dcblocker_t *dc, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	return tt_firstorder_process(dc, inbuf, outbuf);
}

void tt_dcblocker_flush(tt_dcblocker_t *dc)
{
	return tt_firstorder_flush(dc);
}

void tt_dcblocker_setup(tt_dcblocker_t *dc, int freq)
{
	tt_firstorder_highpass(dc, freq);
}
