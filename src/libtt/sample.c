/*
 * sample.c
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

#include <stdio.h>
#include <string.h>

#include "librfn.h"
#include "libtt.h"

#ifdef HAVE_FPU

static void abort_atexit(void)
{
	abort();
}

static void issue_failure_notice(float spl, const char *fname, int lineno)
{
	static bool once_only = false;

	/* report the problem */
	fprintf(stderr, "ttvalidate: %s:%d: Sample range error %.2f\n",
			fname, lineno, spl);

	/* force the problem to terminate abnormally but allow
	 * it to run to completion.
	 */
	if (!once_only) {
		once_only = true;
		atexit(abort_atexit);
	}
}

ttspl_t ttvalidate(ttspl_t spl, const char *fname, int lineno)
{
	if (!TTVALID(spl)) {
		issue_failure_notice(spl, fname, lineno);
	}

	return spl;
}

tlspl_t tlvalidate(tlspl_t spl,  const char *fname, int lineno)
{
	if (!TLVALID(spl)) {
		issue_failure_notice(spl, fname, lineno);
	}

	return spl;
}

#endif // HAVE_FPU

#ifndef HAVE_FPU

ttspl_t ttabs(ttspl_t x)
{
	if (x > 0)
		return x;

	return TTNEGATE(x);
}

ttspl_t ttcos(ttspl_t x)
{
	return ttsin(TTADD(x, TTDINT(TTPI, 2)));
}

ttspl_t ttexp(ttspl_t x)
{
	return TTFLOAT(expf(TTASFLOAT(x)));
}

ttspl_t ttlog2(ttspl_t x)
{
	int z;
	ttspl_t a, c, m, y;

	assert(x);

	//
	// Reminder: This function is part of the the fixed point
	//           implementation. This means we operate on ttspl_t
	//           *without* the macros when this is convenient.
	//

	// Decompose into integer part and fractional part using the summation
	// identity (an identity which remains correct when c is -ve):
	//   logB(a+c) = logB(a) + logB(1 + (c/a))

	// find the integer part of the result by separating the top bit from
	// everything else
	z = ilog2(x);
	a = (1 << z);
	c = x - a;

	// final derivation of the above identity (from now on we'll be working
	// out log2(a) + log2(m)
	m = TTADD(TTINT(1), TLDIV(TTRAISE(c), a));

	// calculating the integer part is easy in base 2. y is currently an
	// integer since it has no fractional part but it does not need to
	// be shifted by TTQ since this is a side effect of the shifts in
	// the bit generating loop.
	y = z - TTQ;

	// now generate each remaining bit
	for (int i=0; i<TTQ; i++) {
		assert(m >= TTINT(1) && m < TTINT(2));

		m = TTMAL(m, m);
		y <<= 1;

		if (m > TTINT(2)) {
			m = TTDINT(m, 2);
			y++;
		}
	}

	return y;
}

ttspl_t ttlog10(ttspl_t x)
{
	const ttspl_t log2_of_10 = 3483294;
	const ttspl_t log10_of_quantization_error = -6628709;

#if 0
	printf("log2_of_10:  %d  (local copy is %d)\n",
			TTFLOAT(log2(10.0)), log2_of_10);
	printf("log10_of_quantization_error: %d  (local copy is %d)\n",
			TTFLOAT(log10(1.0 / (1 << (TTQ+1)))),
			log10_of_quantization_error);
#endif

	// when the argument is 0 the logarithm is infinite. that can't be
	// represented in fixed point and will cause us some numeric
	// problems. we solve this by assuming the first bit removed
	// by the quantizer is non-zero when determining logarithms.
	//
	// One consequence of this is that converting silence into
	// decibels reports ~-126dB.
	if (0 == x)
		return log10_of_quantization_error;

	return TLDIV(TTRAISE(ttlog2(x)), log2_of_10);
}

ttspl_t ttpow(ttspl_t a, ttspl_t b)
{
	return TTFLOAT(pow(TTASFLOAT(a), TTASFLOAT(b)));
}

ttspl_t ttsin(ttspl_t x)
{
	return TTFLOAT(sinf(TTASFLOAT(x)));
}

/**
 * Determine the square root of x using fixed point maths.
 *
 * The algorithm is a simple binary search looking for the greatest value
 * who square is smaller than or equal to x. The only extra trick used is
 * to avoid searching the top most bits (which we already know must be unset).
 *
 * The algorithm will return zero if x is negative (which is no more wrong
 * than any other value).
 */
ttspl_t ttsqrt(ttspl_t x)
{
	tlspl_t target = TTRAISE(x);
	ttspl_t root = 0;
	// TODO: Need a static assert that TTQ == 20
	ttspl_t max = TTINT(32); // gives the highest set bit in root(TTSPL_MAX)

	// binary search skipping to top few bits because they overflow when
	// converted back into a long to standard precision.
	for (ttspl_t bit=max; bit; bit>>=1) {
		ttspl_t candidate = (root | bit);
		tlspl_t square = TTMUL(candidate, candidate);
		if (square <= target)
			root = candidate;
	}

	return root;
	//return TTFLOAT(sqrtf(TTASFLOAT(x)));
}

#endif // !HAVE_FPU
