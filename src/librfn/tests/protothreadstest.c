/*
 * protothreadstest.c
 *
 * Part of libtt (the integer amplifier library)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
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

/* The empty thread should return PT_EXITED every time. */
pt_state_t empty_thread(pt_t *pt)
{
	PT_BEGIN(pt);
	PT_END(pt);
}

/* The yield thread should return PT_YIELDED, then PT_EXITED. */
pt_state_t yield_thread(pt_t *pt)
{
	PT_BEGIN(pt);
	PT_YIELD(pt);
	PT_END(pt);
}

/* The wait thread should cycle through PT_WAITING, PT_WAITING, PT_EXITED. */
pt_state_t wait_thread(pt_t *pt)
{
	static int count = 0;

	PT_BEGIN(pt);
	PT_WAIT_UNTIL(pt, count++ >= 1);
	assert(count >= 1);
	PT_WAIT_UNTIL(pt, --count < 1);
	assert(count == 0);
	PT_END(pt);
}

/* The exit through should return PT_YIELDED then PT_EXITED */
pt_state_t exit_thread(pt_t *pt)
{
	PT_BEGIN(pt);

	while (1) {
		PT_YIELD(pt);
		PT_EXIT(pt);
	}

	PT_WAIT_UNTIL(pt, false);
	PT_END(pt);
}

/* The spawn thread runs though all the other threads so it should
 * return:
 *   Nothing due to empty_thread
 *   PT_YIELDED due to yield_thread
 *   PT_WAITING, PT_WAITING due to wait_thread
 *   PT_YIELDED due to exit_thread
 *   PT_EXITED as it completes.
 */
pt_state_t spawn_thread(pt_t *pt)
{
	static pt_t child;

	PT_BEGIN(pt);

	PT_SPAWN(pt, &child, empty_thread(&child));
	PT_SPAWN(pt, &child, yield_thread(&child));
	PT_SPAWN(pt, &child, wait_thread(&child));
	PT_SPAWN(pt, &child, exit_thread(&child));

	PT_END(pt);
}

int main()
{
	pt_t t;

	PT_INIT(&t);
	verify(PT_EXITED == empty_thread(&t));

	PT_INIT(&t);
	verify(PT_EXITED == empty_thread(&t));

	PT_INIT(&t);
	verify(PT_YIELDED == yield_thread(&t));
	verify(PT_EXITED == yield_thread(&t));

	PT_INIT(&t);
	verify(PT_WAITING == wait_thread(&t));
	verify(PT_WAITING == wait_thread(&t));
	verify(PT_EXITED == wait_thread(&t));

	PT_INIT(&t);
	verify(PT_YIELDED == exit_thread(&t));
	verify(PT_EXITED == exit_thread(&t));

	PT_INIT(&t);
	t = -1;
	verify(PT_CRASHED == exit_thread(&t));

	PT_INIT(&t);
	verify(PT_YIELDED == spawn_thread(&t));
	verify(PT_WAITING == spawn_thread(&t));
	verify(PT_WAITING == spawn_thread(&t));
	verify(PT_YIELDED == spawn_thread(&t));
	verify(PT_EXITED == spawn_thread(&t));

	return 0;
}
