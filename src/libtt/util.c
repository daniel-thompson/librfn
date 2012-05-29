/*
 * util.h
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

#include "libtt.h"

void tt_generic_process(
		ttspl_t (*step)(void *, ttspl_t spl), void *arg,
		tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	assert(inbuf->sz == outbuf->sz);

	for (unsigned i=0; i<inbuf->sz; i++)
		TTAT(outbuf, i) = step(arg, TTAT(inbuf, i));
}

void tt_generic_output(
		ttspl_t (*step)(void *), void *arg,
		tt_sbuf_t *outbuf)
{
	for (unsigned i=0; i<outbuf->sz; i++)
		TTAT(outbuf, i) = step(arg);
}
