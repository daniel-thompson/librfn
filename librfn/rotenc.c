/*
 * rotenc.c
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2016 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#include "librfn/rotenc.h"

void rotenc_decode(rotenc_t *r, uint8_t state)
{
#define FROM(x, y) (((!!x) << 3) + ((!!y) << 2))
#define TO(x, y) (((!!x) << 1) + (!!y))
	uint8_t fromto = (r->last_state << 2) + state;

	switch (fromto) {
	/* clockwise */
	case FROM(0, 0) | TO(0, 1):
	case FROM(0, 1) | TO(1, 1):
	case FROM(1, 1) | TO(1, 0):
	case FROM(1, 0) | TO(0, 0):
		r->internal_count++;
		break;

	/* anticlockwise */
	case FROM(0, 0) | TO(1, 0):
	case FROM(1, 0) | TO(1, 1):
	case FROM(1, 1) | TO(0, 1):
	case FROM(0, 1) | TO(0, 0):
		r->internal_count--;
		break;
	}

	r->last_state = state;
	if (!state)
		r->count = r->internal_count >> 2;
}

uint16_t rotenc_count14(rotenc_t *r)
{
	return ((r->internal_count >> 2) & 0x3f00) + r->count;
}

