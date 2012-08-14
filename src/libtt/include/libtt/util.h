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

#include <assert.h>

#include "librfn/util.h" // for xmalloc (used by generic macros)

/**
 * Assigned a unique number to each of the high level processing block.
 *
 * Primarily this is used to partition the control space to make
 * composition easier.
 */
typedef enum {
	TT_TUBESTAGE_BASE = 1,
	TT_PREAMP_BASE,
	TT_CABSIM_BASE,
	TT_TINTAMP_BASE
} tt_base_t;

#define TT_BASE2TAG(x) ((x) << 16)
#define TT_TAG2BASE(x) ((x) >> 16)
#define TT_TAG2ID(x)   ((x) & 0xffff)

typedef struct {
	unsigned int sampling_frequency;
	unsigned int grain_size;
} tt_context_t;

void tt_context_init(tt_context_t *ctx);
static inline void tt_context_finalize(tt_context_t *ctx) {}
tt_context_t *tt_context_new();
void tt_context_delete(tt_context_t *ctx);

#define tt_generic_new(type) \
tt_##type##_t *tt_##type##_new(tt_context_t *ctx) \
{ \
	tt_##type##_t *p = xmalloc(sizeof(*p)); \
	tt_##type##_init(p, ctx); \
	return p; \
}

#define tt_generic_delete(type) \
void tt_##type##_delete(tt_##type##_t *p) \
{ \
	tt_##type##_finalize(p); \
	free(p); \
}

/*!
 * Generate a simple control enumeration function.
 *
 * The generated function is only of use for build blocks that have a single
 * contiguous range of control values.
 */
#define tt_generic_enum_control(type, min, max) \
tt_##type##_control_t tt_##type##_enum_control(tt_##type##_control_t ctrl) \
{ \
	return (ctrl < (min)  ? (min) : \
		ctrl >= (max) ? 0 : \
			        ctrl+1); \
}

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

/*!
 * Generate a 1-in/0-out process function from an appropriate step
 * function.
 */
#define tt_generic_input(type) \
void tt_##type##_process(tt_##type##_t *p, tt_sbuf_t *inbuf) \
{ \
	for (unsigned i=0; i<inbuf->sz; i++) \
		tt_##type##_step(p, TTAT(inbuf, i)); \
}

#endif // TT_UTIL_H_
