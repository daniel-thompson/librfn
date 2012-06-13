/*
 * analyse.c
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

#include "libtt.h"

/*! Report the (absolute) peak value observed in a buffer.
 *
 */
ttspl_t tt_analyse_peak(tt_sbuf_t *sbuf)
{
	ttspl_t peak = TTINT(0);

	for (unsigned i=0; i<sbuf->sz; i++) {
		ttspl_t spl = TTAT(sbuf, i);
		ttspl_t abspl = TTABS(spl);

		if (abspl > peak)
			peak = abspl;
	}

	return peak;
}

/*! Report the (absolute) mean average value in a buffer.
 *
 */
ttspl_t tt_analyse_rectify(tt_sbuf_t *sbuf)
{
	tlspl_t acc = TLINT(0);

	for (unsigned i=0; i<sbuf->sz; i++) {
		ttspl_t spl = TTAT(sbuf, i);
		tlspl_t labspl = TTRAISE(TTABS(spl));
		acc = TLADD(acc, labspl);
	}

	return TLLOWER(TLDINT(acc, sbuf->sz));
}

/*! Check a buffer for any illegal values.
 */
bool tt_analyse_valid(tt_sbuf_t *sbuf)
{
	bool valid = true;

	for (unsigned i=0; i<sbuf->sz; i++)
		valid = valid && TTVALID(TTAT(sbuf, i));

	return valid;
}
