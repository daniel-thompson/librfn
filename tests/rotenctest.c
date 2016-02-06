/*
 * rotenctest.c
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

#undef NDEBUG

#include <librfn.h>

int main()
{
#define STEP(x, y) rotenc_decode(&myenc, (x << 1) + y)
	rotenc_t myenc = ROTENC_VAR_INIT;

	verify(0 == rotenc_count(&myenc));
	verify(0 == rotenc_count14(&myenc));

	/* no turn */
	STEP(0, 0);
	verify(0 == rotenc_count(&myenc));
	verify(0 == rotenc_count14(&myenc));

	/* turn cw */
	STEP(0, 1);
	verify(0 == rotenc_count(&myenc));
	verify(0 == rotenc_count14(&myenc));
	STEP(1, 1);
	verify(0 == rotenc_count(&myenc));
	verify(0 == rotenc_count14(&myenc));
	STEP(1, 0);
	verify(0 == rotenc_count(&myenc));
	verify(0 == rotenc_count14(&myenc));
	STEP(0, 0);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));

	/* bounce */
	STEP(0, 1);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));
	STEP(0, 0);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));
	STEP(1, 0);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));
	STEP(0, 0);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));

	/* noise */
	STEP(1, 1);
	STEP(0, 0);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));

	/* turn ccw */
	STEP(1, 0);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));
	STEP(1, 1);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));
	STEP(0, 1);
	verify(1 == rotenc_count(&myenc));
	verify(1 == rotenc_count14(&myenc));
	STEP(0, 0);
	verify(0 == rotenc_count(&myenc));
	verify(0 == rotenc_count14(&myenc));

	/* wrapped count */
	STEP(1, 0);
	STEP(1, 1);
	STEP(0, 1);
	STEP(0, 0);
	verify(255 == rotenc_count(&myenc));
	verify(16383 == rotenc_count14(&myenc));

	/* extremely wrapped count */
	for (int i=0; i<1024; i++) {
		STEP(1, 0);
		STEP(1, 1);
		STEP(0, 1);
		STEP(0, 0);
	}
	verify(255 == rotenc_count(&myenc));
	verify(16383-1024 == rotenc_count14(&myenc));

	/* cw turn */
	STEP(0, 1);
	STEP(1, 1);
	STEP(1, 0);
	STEP(0, 0);
	verify(0 == rotenc_count(&myenc));
	verify(16383-1023 == rotenc_count14(&myenc));

	return 0;
}
