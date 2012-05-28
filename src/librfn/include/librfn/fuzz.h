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

int fuzzcmp(double a, double b, double delta);
int fuzzcmpf(float a, float b, float delta);

int fuzzcmpb(double a, double b, int bits);
int fuzzcmpbf(float a, float b, int bits);

#endif // RF_FUZZ_H_
