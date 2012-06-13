/*
 * wavheadertest.c
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

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <librfn.h>

int main()
{
	rf_wavheader_t in, out;
	int insz, outsz;
	char *s;
	uint8_t header[128];

	memset(&in, 0, sizeof(in));
	assert(0 != rf_wavheader_validate(&in));

	rf_wavheader_init(&in, 44100, 1, RF_WAVHEADER_FLOAT);
	assert(0 == rf_wavheader_validate(&in));

	s = rf_wavheader_tostring(&in);
	printf("%s\n", s);
	free(s);

	insz = rf_wavheader_encode(&in, header, sizeof(header));
	printf("Encoded header size %d\n", insz);
	assert(insz < sizeof(header));

	outsz = rf_wavheader_decode(header, insz, &out);
	printf("Decoded header size %d\n", outsz);
	assert(insz == outsz);
	assert(0 == memcmp(&in, &out, sizeof(in)));

	return 0;
}
