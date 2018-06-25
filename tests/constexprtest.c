/*
 * consttest.c
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2018 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#undef NDEBUG
#define VERBOSE

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <librfn.h>

/*
 * Compile time tests to prove that the macros really are implemented a
 * a pure constant expression.
 */
enum { RF_CONSTEXPR_POP_TEST = const_pop(0xffff) };
enum { RF_CONSTEXPR_LSSB_TEST = const_lssb(0xffff) };


int main()
{
	verify(const_lssb(         0) == -1);
	verify(const_lssb(         0) == -1);
	verify(const_lssb(         1) ==  0);
	verify(const_lssb(         2) ==  1);
	verify(const_lssb(0xffffffff) ==  0);
	verify(const_lssb(0xffffff00) ==  8);
	verify(const_lssb(0x00000f00) ==  8);
	verify(const_lssb(0x80010000) == 16);
	verify(const_lssb(0x80000000) == 31);

	verify(const_lssb(0xffffffffffffffffull) == 0);
	verify(const_lssb(0x8000000000000000ull) == 63);
	verify(const_lssb(0x0000000000000000ull) == -1);
	verify(const_lssb(0x0000000000000001ull) == 0);
	verify(const_lssb(0x0000000000000002ull) == 1);
	verify(const_lssb(0x0000000000000004ull) == 2);
	verify(const_lssb(0x0000000000000008ull) == 3);
	verify(const_lssb(0x0000000000000010ull) == 4);
	verify(const_lssb(0x0000000000000020ull) == 5);
	verify(const_lssb(0x0000000000000040ull) == 6);
	verify(const_lssb(0x0000000000000080ull) == 7);


	verify(const_pop(         0) ==  0);
	verify(const_pop(0x12481248) ==  8);
	verify(const_pop(0x0000ffff) == 16);
	verify(const_pop(0xffffffff) == 32);

	verify(const_pop(0x0123456789abcdefull) == 32);
	verify(const_pop(0xccccccccccccccccull) == 32);
	verify(const_pop(0xeeeeeeeeeeeeeeeeull) == 48);
	verify(const_pop(0xffffffffffffffffull) == 64);

	return 0;
}
