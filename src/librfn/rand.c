/*
 * rand.c
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

#include <assert.h>
#include <stdint.h>

/*! \todo Remove this include... its not needed once rand31 is correctly implemented */
#include <stdlib.h>

#include "librfn/rand.h"

/*!
 * Generate a 31-bit random number.
 *
 * From http://www.firstpr.com.au/dsp/rand31/ :
 * Park-Miller "minimal standard" 31 bit
 * pseudo-random number generator, implemented
 * with David G. Carta's optimization: with
 * 32 bit math and without division.
 *
 * Nth generated number (starting from zero) should be ???
 *
 * \param [inout] seedp Current state of the PRNG, initalized using
 *                      ::RAND31_VAR_INIT
 */
uint32_t rand31_r(uint32_t *seedp)
{
	uint32_t hi, lo;

	lo = 16807 * (*seedp & 0xffff);
	hi = 16807 * (*seedp >> 16);

	lo += (hi & 0x7fff) << 16;
	lo += hi >> 15;

	if (lo > 0x7fffffff)
		lo -= 0x7fffffff;

	return (*seedp = lo);
}

