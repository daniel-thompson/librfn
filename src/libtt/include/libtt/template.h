/*
 * template.h
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

#ifndef TT_TEMPLATE_H_
#define TT_TEMPLATE_H_

#include "libtt/util.h"

typedef struct {
	tt_context_t *ctx;

} tt_template_t;

void tt_template_init(tt_template_t *p, tt_context_t *ctx);
void tt_template_finalize(tt_template_t *p);
tt_template_t *tt_template_new(tt_context_t *ctx);
void tt_template_delete(tt_template_t *p);

void tt_template_setup(tt_template_t *p);

ttspl_t tt_template_step(tt_template_t *p, ttspl_t spl);
void tt_template_process(tt_template_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf);

#endif // TT_TEMPLATE_H_
