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

/*!
 * Generate a 1-in/1-out process function from an appropriate step
 * function.
 *
 * This macro is designed to take advantage of an inlined step function
 * giving the compiler all the information it needs to work hard at
 * optimizing the loop.
 */
#define tt_generic_process(type) \
void tt_##type##_process(tt_##type##_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf) \
{ \
	assert(inbuf->sz <= outbuf->sz); \
	for (unsigned i=0; i<inbuf->sz; i++) \
		TTAT(outbuf, i) = tt_##type##_step(p, TTAT(inbuf, i)); \
}

/*!
 * Generate a 0-in/1-out process function from an appropriate step
 * function.
 */
#define tt_generic_output(type) \
void tt_##type##_process(tt_##type##_t *p, tt_sbuf_t *outbuf) \
{ \
	for (unsigned i=0; i<outbuf->sz; i++) \
		TTAT(outbuf, i) = tt_##type##_step(p); \
}

#endif // TT_UTIL_H_
