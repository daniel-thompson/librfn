/*
 * hex.c
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

static inline char hexchar(char h)
{
	if (h < 10)
		return '0' + h;

	return 'a' - 10 + h;
}

int rf_hexdump(unsigned char *p, size_t sz)
{
	return rf_fhexdump(stdout, p, sz);
}

int rf_fhexdump(FILE *f, unsigned char *p, size_t sz)
{
	size_t oldsz = sz;

	while (sz > 0) {
		for (int i=0;
		     i<16 && sz > 0;
		     i++, sz--, p++)
			fprintf(f, "%c%c", hexchar(*p >> 4), hexchar(*p & 0xf));
		fprintf(f, "\n");
	}

	return oldsz;
}
