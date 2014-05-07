/*
 * messageq.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2013 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_MESSAGEQ_H_
#define RF_MESSAGEQ_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "atomic.h"

typedef struct {
	char *basep;
	uint16_t msg_len;
	unsigned char queue_len;

	atomic_uchar num_free;
	atomic_uchar sendp;
	atomic_uint full_flags;

	unsigned char receivep;

} messageq_t;

#define MESSAGEQ_VAR_INIT(basep, base_len, msg_len) \
	{ \
		(char *) (basep), \
		(msg_len), \
		((base_len) / (msg_len)), \
		ATOMIC_VAR_INIT(((base_len) / (msg_len))), \
		ATOMIC_VAR_INIT(0), \
		ATOMIC_VAR_INIT(0), \
		0 \
	}

void messageq_init(messageq_t *mq, void *basep, size_t base_len, size_t msg_len);

void *messageq_claim(messageq_t *mq);
void messageq_send(messageq_t *mq, void *msg);
bool messageq_empty(messageq_t *mq);
void *messageq_receive(messageq_t *mq);
void messageq_release(messageq_t *mq, void *msg);

#endif // RF_MESSAGEQ_H_
