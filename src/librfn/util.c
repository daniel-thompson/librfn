/*
 * util.c
 *
 * Red Feline Ninja General Library
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
