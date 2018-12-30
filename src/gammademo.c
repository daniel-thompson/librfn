/*
 * gammademo.c
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2019 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "librfn.h"

static void tablegen(double gamma)
{
	/* narrow the scale because we want 0 to be a special case */
	const int scale_in = 254;
	const int scale_out = 254;

	printf("const rgb_gamma_t gamma = {\n");
	printf("\t{\n\t\t");
	printf("  0, ");
	for (int i=0; i<=scale_in; i++) {
		if (((i + 1) % 12) == 0)
			printf("\n\t\t");
		printf("%3d, ",
			(int) (pow((double) i / scale_in, gamma) * scale_out + 1.5));
	}
	printf("\n\t}\n};\n");
}

int main(int argc, char *argv[])
{
	tablegen(2.2);

	printf("\nSingle LED\n\n");
	for (int i=0; i<256; i+=8)
		printf("%6x -> %6x\n", i, rgb_correct(&rgb_gamma_default, i));
	
	printf("\nFull RGB\n\n");
	for (int i=0; i<256; i+=8)
		printf("%06x -> %06x\n", i + (i << 8) + (i << 16),
		       rgb_correct(&rgb_gamma_default,
			       i + (i << 8) + (i << 16)));

	return 0;
}
