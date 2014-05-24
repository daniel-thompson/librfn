/*
 * console.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2014 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_CONSOLE_H_
#define RF_CONSOLE_H_

#include <stdio.h>

#include "fibre.h"
#include "ringbuf.h"

/*!
 * \defgroup librfn_console Command console
 *
 * \brief Lightweight command interpreter implemented as a protothreads
 *
 * The use of protothreads means this command interpreter can be used effectively
 * on run-to-completion schedulers and comes with out of the box support for fibres.
 *
 * The use of the fibre scheduler is optional (although the fibre header file is
 * needed to compile source without modification).
 *
 * @{
 */

struct console;
typedef pt_state_t console_cmd_t(struct console *c);

#define SCRATCH_SIZE 80

typedef struct console {
	fibre_t fibre;

	FILE *out;

	char ringbuf[16];
	ringbuf_t ring;

	/*!
	 * Scratch buffer used by commands to store state.
	 *
	 * \warning Arguments passed contain pointers to these scratch buffers.
	 *          Thus commands must parse their command line before storing state
	 *          in the scratch buffers.
	 */
	union {
		char buf[SCRATCH_SIZE];
		uint8_t u8[SCRATCH_SIZE];
		uint16_t u16[SCRATCH_SIZE/2];
		uint32_t u32[SCRATCH_SIZE/4];
		void *p[SCRATCH_SIZE/4];
	} scratch;
	char *bufp;

	int argc;
	char *argv[4];

	console_cmd_t *cmd;
	pt_t pt;
} console_t;

void console_init(console_t *c, FILE *f);

/*!
 * \brief Platform dependant function that will be called during console_init().
 */
void console_hwinit(console_t *c);

int console_register(const char *name, console_cmd_t cmd);

void console_putchar(console_t *c, char d);

pt_state_t console_run(console_t *c);

/*! @} */
#endif // RF_CONSOLE_H_
