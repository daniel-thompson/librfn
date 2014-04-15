/*
 * util.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2012 Daniel Thompson <daniel@redfelineninja.org.uk> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_UTIL_H_
#define RF_UTIL_H_

#include <assert.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * This macro is similar to the Linux kernel's container_of() macro but
 * doesn't use the GNU extension needed to assure type safety.
 */
#define containerof(ptr, type, member) \
	((type *) (((char *) ptr) - offsetof(type, member)))

/**
 * Determine the number of elements in a statically allocated array.
 *
 * There's no compile-time checks to this macro. Abuse it and you'll just
 * get the wrong result!
 */
#define lengthof(x) ((sizeof(x) / sizeof(*(x))))

#ifdef VERBOSE
#define verify(s) \
	do { \
		printf("Checking %s ... ", #s); \
		assert(s); \
		printf("OK\n"); \
	} while (0)

#else
#define verify(x) assert(x)
#endif

/*!
 * @returns >1 if a > b
 * @returns 0 if a == b
 * @returns <1 if a < b
 */
int32_t cyclecmp32(uint32_t a, uint32_t b);

void rf_internal_out_of_memory(void);

void *xmalloc(size_t sz);

char *strdup_printf(const char *format, ...);
char *strdup_vprintf(const char *format, va_list ap);
char *xstrdup_printf(const char *format, ...);
char *xstrdup_vprintf(const char *format, va_list ap);

#endif // RF_UTIL_H_
