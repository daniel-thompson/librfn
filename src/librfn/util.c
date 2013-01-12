/*
 * util.c
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2012 Daniel Thompson <daniel@redfelineninja.org.uk> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdlib.h>
#include <stdio.h>

#include "librfn.h"

int32_t cyclecmp32(uint32_t a, uint32_t b)
{
	return (int) (a - b);
}

/*! Out of memory hook.
 *
 * \todo Currently out_of_memory cannot actually be hooked
 */
void rf_internal_out_of_memory(void)
{
	abort();
}

void *xmalloc(size_t sz)
{
	void *p = malloc(sz);
	if (!p)
		rf_internal_out_of_memory();
	return p;
}

char *strdup_printf(const char *fmt, ...)
{
	char *str;
	va_list ap;

	va_start(ap, fmt);
	str = strdup_vprintf(fmt, ap);
	va_end(ap);

	return str;
}

char *strdup_vprintf(const char *fmt, va_list ap)
{
	char *str;
	int len;
	va_list nap;

	va_copy(nap, ap);
	len = vsnprintf(NULL, 0, fmt, nap);
	va_end(nap);

	str = malloc(len+1);
	if (str)
		vsprintf(str, fmt, ap);

	return str;
}

char *xstrdup_printf(const char *fmt, ...)
{
	char *str;
	va_list ap;

	va_start(ap, fmt);
	str = xstrdup_vprintf(fmt, ap);
	va_end(ap);

	return str;
}

char *xstrdup_vprintf(const char *fmt, va_list ap)
{
	char *str;
	int len;
	va_list nap;

	va_copy(nap, ap);
	len = vsnprintf(NULL, 0, fmt, nap);
	va_end(nap);

	str = xmalloc(len+1);
	if (str)
		vsprintf(str, fmt, ap);

	return str;
}
