/*
 * protothreads.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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
 * al but with a few small tweaks.
 *
 * 1. Stronger distinction between yielding and waiting. This allows a
 *    scheduler to handle the cases differently. The fibre scheduler included
 *    in this library observes this distinction. We also eliminate the local
 *    continuation macros concept allowing a implementation of PT_YIELD()
 *    with no state variable.
 *
 * 2. If the thread state variable is inconsistent the thread reports
 *    PT_CRASHED to its caller. This makes detecting use of the
 *    PT_WAIT_UNTIL() macro from within nested switch statements more
 *    likely.
 *
 * 3. The use of the provided pt_t type is optional. Any integer type large
 *    enough not to overflow when __LINE__ is assigned to it is sufficient.
 */

typedef enum {
	PT_WAITING,
	PT_YIELDED,
	PT_EXITED,
	PT_CRASHED
} pt_state_t;

typedef uint16_t pt_t;

#define PT_INIT(pt) \
	do { \
		*(pt) = 0; \
	} while (0)

#define PT_BEGIN(pt) \
	switch (*(pt)) { \
	case 0:

#define PT_END(pt) \
	break; \
	default: return PT_CRASHED; \
        } \
	return PT_EXITED

#define PT_WAIT_UNTIL(pt, c) \
	do { \
		*(pt) = __LINE__; \
                case __LINE__: \
		if (!(c)) \
			return PT_WAITING; \
	} while (0)

#define PT_YIELD(pt) \
	do { \
		*(pt) = __LINE__; \
		return PT_YIELDED; \
		case __LINE__: ;\
	} while (0)

#define PT_EXIT(pt) \
	return PT_EXITED

#define PT_SPAWN(pt, child, thread) \
	do { \
		pt_state_t ptres; \
		PT_INIT(child); \
		*(pt) = __LINE__; \
                case __LINE__: \
                ptres = (thread); \
                if (ptres != PT_EXITED) \
                	return ptres; \
	} while (0)

#endif // RF_PROTOTHREADS_H_
