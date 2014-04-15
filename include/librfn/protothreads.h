/*
 * protothreads.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_PROTOTHREADS_H_
#define RF_PROTOTHREADS_H_

#include <stdint.h>

/*
 * Like all protothreads implementations this work is inspired by the paper
 * "Protothreads: Simplifying Event-Driven Programming of Memory-Constrained
 * Embedded Systems" by Dunkels et al.
 *
 * This implementation uses a similar interface as that proposed by Dunkels et
 * al but with sufficient changes to make them incompatible:
 *
 * 1. Stronger distinction between yielding and waiting. This allows a
 *    scheduler to handle the cases differently. The fibre scheduler included
 *    in this library observes this distinction. We also eliminate the local
 *    continuation macros concept allowing a implementation of PT_YIELD()
 *    with no state variable.
 *
 * 2. PT_INIT() saves the protothread pointer to simplify the usage of the other
 *    macros.
 *
 * 3. The use of the provided pt_t type is optional. Any integer type large
 *    enough not to overflow when __LINE__ is assigned to it is sufficient.
 */

typedef enum {
	PT_YIELDED,
	PT_WAITING,
	PT_EXITED
} pt_state_t;

typedef uint16_t pt_t;

#define PT_INIT(pt) \
	do { \
		*(pt) = 0; \
	} while (0)

#define PT_BEGIN(pt) \
	{ \
		pt_t *protothread_pointer_is_not_defined_missing_PT_BEGIN = pt; \
		(void) protothread_pointer_is_not_defined_missing_PT_BEGIN; \
		switch (*protothread_pointer_is_not_defined_missing_PT_BEGIN) { \
		case 0:

#define PT_END() \
			break; \
		default: \
			assert(0); \
		} \
        } \
	return PT_EXITED

#define PT_WAIT_UNTIL(c) \
	do { \
		*protothread_pointer_is_not_defined_missing_PT_BEGIN = __LINE__; \
                case __LINE__: \
		if (!(c)) \
			return PT_WAITING; \
	} while (0)

#define PT_YIELD() \
	do { \
		*protothread_pointer_is_not_defined_missing_PT_BEGIN = __LINE__; \
		return PT_YIELDED; \
		case __LINE__: ;\
	} while (0)

#define PT_EXIT() \
	return PT_EXITED

#define PT_SPAWN(child, thread) \
	do { \
		pt_state_t ptres; \
		PT_INIT(child); \
		*protothread_pointer_is_not_defined_missing_PT_BEGIN = __LINE__; \
                case __LINE__: \
                ptres = (thread); \
                if (ptres != PT_EXITED) \
                	return ptres; \
	} while (0)

#endif // RF_PROTOTHREADS_H_
