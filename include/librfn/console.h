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
 * \note The use of the fibre scheduler is optional (although the fibre header file
 *       is needed to compile source without modification).
 *
 * No dynamic memory allocation is required by the console handling (although
 * currently it uses stdio.
 *
 * @{
 */

struct console;
typedef pt_state_t console_cmd_t(struct console *c);

#define SCRATCH_SIZE 80

/*!
 * \brief Console descriptor.
 *
 * This is a relatively large structure (164 bytes on a 32-bit machine) due
 * to the integrated scratch array.
 */
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

/*!
 * \brief Initialized the console handler.
 *
 * \param c Pointer to console descriptor
 * \param f File pointer to be used for all console output
 */
void console_init(console_t *c, FILE *f);

/*!
 * \brief Platform dependant function that will be called during console_init().
 *
 * librfn provides example implementations of this function but, in some cases,
 * this function must be provided by the application.
 */
void console_hwinit(console_t *c);

/*!
 * \brief Register a new command.
 *
 * The following example shows a simple protothreaded command to list the command's
 * arguments:
 *
 * \code
 * pt_state_t listargs(console_t *c)
 * {
 *     PT_BEGIN(&c->pt);
 *     for (i=0; i<c->argc; i++) {
 *         fprintf(c->out, "%d: %s\n", i, c->argv[i]);
 *         PT_YIELD();
 *     }
 *     PT_END();
 * }
 *
 * (void) console_register("listargs", listargs);
 * \endcode
 *
 * The use of protothreading is optional. The following command is functionally
 * equivalent although may causes a run-to-completion scheduler may run poorly if
 * fprintf() is slow (for example if it synchronously sends characters to a serial
 * port):
 *
 * \code
 * pt_state_t listargs(console_t *c)
 * {
 *     for (i=0; i<c->argc; i++)
 *         fprintf(c->out, "%d: %s\n", i, c->argv[i]);
 *     return PT_EXITED;
 * }
 * \endcode
 */
int console_register(const char *name, console_cmd_t cmd);

/*!
 * \brief Asynchronously send a character to the command processor.
 *
 * This function is safe to call from interrupt. It will insert a character into
 * the command processors ring buffer and will, optionally, schedule the console
 * fibre.
 */
void console_putchar(console_t *c, char d);

/*!
 * \brief Console protothread entrypoint.
 *
 * This function can be used to integrate the command processor into simple
 * polling loops or to "alien" run-to-completion schedulers.
 *
 * The return code can be used to realize power saving modes. However a simple
 * polling loop without support for power saving can simply ignore it.
 *
 * \code
 * while (true) {
 *     poll_something();
 *     (void) console_run(console); // poll console
 *     poll_something_else();
 * }
 * \endcode
 */
pt_state_t console_run(console_t *c);

/*!
 * \brief Synchronous console function for use in threaded environments.
 *
 * This function can also be used to implement a command console that executes
 * from an interrupt handler. Such a command interpreter would be extremely robust
 * although potentially at the cost of poor interrupt latencies.
 *
 * To use this function console_hwinit() may have to altered to remove code that
 * asynchronously delivers characters.
 *
 * \code
 * while ((ch = getchar()) != -1)
 *     console_process(console, ch);
 * \endcode
 */
static inline void console_process(console_t *c, char d)
{
	console_putchar(c, d);
	pt_state_t s;
	do {
		s = console_run(c);
	} while (s == PT_YIELDED);
}

/*! @} */
#endif // RF_CONSOLE_H_
