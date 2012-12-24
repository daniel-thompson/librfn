/*
 * bitops.h
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

#ifndef RF_BITOPS_H_
#define RF_BITOPS_H_

#include <stdint.h>

/*!
 * Count the number of set bits in a 32-bit word.
 *
 * Algorithm comes from Hacker's Delight by Henry S. Warren, Jr
 */
int bitcnt(uint32_t x);

/*!
 * Count leading zeros.
 */
int clz(uint32_t x);

/*!
 * Integer logarithm in base 2.
 */
int ilog2(uint32_t x);

#endif // RF_BITOPS_H_
