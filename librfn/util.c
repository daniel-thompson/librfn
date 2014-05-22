/*
 * util.c
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
