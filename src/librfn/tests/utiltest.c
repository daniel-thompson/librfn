/*
 * utiltest.c
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

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <librfn.h>

struct alpha {
	int a;
} a;

struct bravo {
	struct alpha a;
	int b;
} b;

struct charlie {
	struct alpha a;
	struct bravo b;
	int c;
} c;

int main()
{
	struct alpha *ap = &b.a;
	verify(&b == containerof(ap, struct bravo, a));

	struct bravo *bp = &c.b;
	verify(&c == containerof(bp, struct charlie, b));

	return 0;
}
