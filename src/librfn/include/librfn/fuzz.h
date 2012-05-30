/*
 * fuzz.h
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

#ifndef RF_FUZZ_H_
#define RF_FUZZ_H_

#include <assert.h>

int fuzzcmp(double a, double b, double delta);
int fuzzcmpf(float a, float b, float delta);

int fuzzcmpb(double a, double b, int bits);
int fuzzcmpbf(float a, float b, int bits);

/*! Assert two values are approximately equal.
 *
 * Currently this checks the values differ by no more than 10 parts
 * per million.
 */
#define asserteq(a, b) assert(fuzzcmp(a, b, 1.000010))

#endif // RF_FUZZ_H_
