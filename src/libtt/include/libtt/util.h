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

#ifndef TT_UTIL_H_
#define TT_UTIL_H_

void tt_generic_process(
		ttspl_t (*step)(void *, ttspl_t spl), void *arg,
		tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_UTIL_H_
