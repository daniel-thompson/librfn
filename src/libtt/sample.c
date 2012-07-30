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
