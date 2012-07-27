/*
 * sampletest.c
 *
 * Part of libtt (the integer amplifier library)
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

#include <librfn.h>
#include <libtt.h>

int main()
{
	float minus6 = TTASFLOAT(TTLINEAR2DB(TTFLOAT(0.5)));
	printf("minus6 = %5.2fdB\n", minus6);
	assert(fuzzcmpf(-6, minus6, 1.01));

	float quarter = TTASFLOAT(TTDB2LINEAR(TTINT(-12)));
	printf("quarter = %9.6f\n", quarter);
	assert(fuzzcmpf(0.25, quarter, 1.01));

#define T(name, expression, expected) \
{ \
	ttspl_t tt ## name = expression; \
	float name = TTASFLOAT(tt ## name); \
	printf("%s = %9.6f (expected %9.6f)\n", #name, name, expected); \
	assert(fuzzcmpf(name, expected, 1.0001)); \
}

	// test the conversions
	T(tenth, TTFLOAT(0.1), 0.1); // TTFLOAT
	T(ten, TTINT(10), 10.0); // TTINT
	T(four, TTINT(TTASINT(TTFLOAT(4.5))), 4.0); // TTASINT
	T(half, TTS16LE(0x3fff), 0.5); // TTS16LE
	T(hundred, TTINT(TTASS16LE(TTS16LE(100))), 100.0); // TTASS16LE
	T(half, TTS32LE(0x3fffffff), 0.5); // TTS32LE
	T(thousand, TTINT(TTASS32LE(TTS32LE(1000))), 1000.0); // TTASS16LE
	// TODO: TTRAISE

	// basic maths
	T(fortytwo, TTADD(TTFLOAT(22.2), TTFLOAT(19.8)), 42.0); // TTADD
	T(six, TTSUB(TTFLOAT(10.7), TTFLOAT(4.7)), 6.0); // TTSUB
	T(minus4, TTNEGATE(TTFLOAT(4.2)), -4.2); // TTNEGATE
	T(hundredth, TLLOWER(TTMUL(TTFLOAT(0.1), TTFLOAT(0.1))), 0.01); // TTMUL
	T(sixteen, TTMAL(TTINT(4), TTINT(4)), 16.0); // TTMAL
	T(twenty, TTMINT(TTINT(5), 4), 20.0); // TTMINT
	T(hundreth, TTDIV(TTFLOAT(0.1), TTINT(10)), 0.01); // TTDIV
	T(hundreth, TTRAD(TTFLOAT(0.1), TTINT(10)), 0.01); // TTRAD
	T(hundreth, TTDINT(TTFLOAT(0.1), 10), 0.01); // TTDINT
	T(five, TTADDI(TTFLOAT(-1.0), 6), 5.0); // TTADDI
	T(minus1, TTSUBI(TTINT(2), 3), -1.0); // TTSUBI
	T(minus1, TTISUB(2, TTINT(3)), -1.0); // TTISUB

	// multiply-accumulate
	tlspl_t acc = TLINT(0);
	TTMAC(acc, TTINT(1), TTINT(1));
	TTMAC(acc, TTINT(2), TTINT(2));
	TTMAC(acc, TTINT(3), TTINT(3));
	TTMAC(acc, TTINT(4), TTINT(4));
	T(sum, TLLOWER(acc), 1.0 + 4.0 + 9.0 + 16.0); // TTMAC

	// constants
	T(pi, TTPI, 3.14159); // TTPI

	// functions
	T(one, TTABS(TTINT(-1)), 1.0); // TTABS
	T(fcos, TTCOS(TTPI), -1.0); // TTCOS
	T(flog2, TTLOG2(TTINT(16)), 4.0); // TTLOG2
	T(flog10, TTLOG10(TTINT(1000)), 3.0); // TTLOG10
	T(fpow, TTPOW(TTINT(2), TTINT(4)), 16.0); // TTPOW
	T(fsin, TTSIN(TTPI/2), 1.0); // TTSIN
	T(fsqrt, TTSQRT(TTINT(49)), 7.0); // TTSQRT

	// validity checks
	T(valid, TTINT(TTVALID(TTINT(0))), 1.0); // TTVALID

	// long conversions
	T(lfloat, TLLOWER(TLFLOAT(-3.14)), -3.14); // TLFLOAT
	T(lint, TLLOWER(TLINT(100)), 100.0); // TLINT
	// TLLOWER() does not have an independant test

	// long maths
	T(fortytwo, TLLOWER(TLADD(TLFLOAT(22.2), TLFLOAT(19.8))), 42.0); // TLADD
	T(six, TLLOWER(TLSUB(TLFLOAT(10.7), TLFLOAT(4.7))), 6.0); // TLSUB
	T(hundreth, TLLOWER(TLDIV(TLFLOAT(0.1), TLINT(10))), 0.01); // TLDIV
	T(hundreth, TLLOWER(TLDINT(TLFLOAT(0.1), 10)), 0.01); // TLDINT

	return 0;
}
