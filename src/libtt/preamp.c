/*
 * preamp.c
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

#include <string.h>

#include "librfn.h"
#include "libtt.h"

void tt_preamp_init(tt_preamp_t *p, tt_context_t *ctx)
{
	memset(p, 0, sizeof(*p));

	p->ctx = ctx;

	for (int i=0; i<lengthof(p->stages); i++)
		tt_tubestage_init(&(p->stages[i]), ctx);

	p->tmpbuf = tt_sbuf_new(ctx->grain_size);
}

void tt_preamp_finalize(tt_preamp_t *p)
{
	for (int i=0; i<lengthof(p->stages); i++)
		tt_tubestage_finalize(&(p->stages[i]));

	tt_sbuf_delete(p->tmpbuf);
}

tt_generic_new(preamp);
tt_generic_delete(preamp);

void tt_preamp_setup(tt_preamp_t *p, tt_preamp_model_t model)
{
	tt_tubestage_setup(&(p->stages[0]), TT_TUBESTAGE_12AX7_Ri68K, -4, 2700, 22570, 86);
	tt_tubestage_setup(&(p->stages[1]), TT_TUBESTAGE_12AX7_Ri250K, -9, 1500, 6531, 132);
	tt_tubestage_setup(&(p->stages[2]), TT_TUBESTAGE_12AX7_Ri250K, -14, 820, 6531, 194);

	p->num_stages = 3;
}

ttspl_t tt_preamp_step(tt_preamp_t *p, ttspl_t spl)
{
	for (int i=0; i<p->num_stages; i++)
		spl = tt_tubestage_step(&(p->stages[i]), spl);
	return spl;
}

void tt_preamp_process(tt_preamp_t *p, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	switch(p->num_stages) {
	case 1:
		tt_tubestage_process(&(p->stages[0]), inbuf, outbuf);
		break;
	case 2:
		tt_tubestage_process(&(p->stages[0]), inbuf, p->tmpbuf);
		tt_tubestage_process(&(p->stages[1]), p->tmpbuf, outbuf);
		break;
	case 3:
		tt_tubestage_process(&(p->stages[0]), inbuf, outbuf);
		tt_tubestage_process(&(p->stages[1]), outbuf, p->tmpbuf);
		tt_tubestage_process(&(p->stages[2]), p->tmpbuf, outbuf);
		break;
	default:
		assert(0);
		break;
	}
}
