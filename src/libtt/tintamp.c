/*
 * tintamp.c
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

void tt_tintamp_init(tt_tintamp_t *tt, tt_context_t *ctx)
{
	memset(tt, 0, sizeof(*tt));
	tt->ctx = ctx;
	tt_preamp_init(&tt->preamp, ctx);
	tt_cabsim_init(&tt->cabsim, ctx);
	tt->tmpbuf = tt_sbuf_new(ctx->grain_size);

	tt_preamp_setup(&tt->preamp, TT_PREAMP_CLEAN);
}

void tt_tintamp_finalize(tt_tintamp_t *tt)
{
	tt_preamp_finalize(&tt->preamp);
	tt_cabsim_finalize(&tt->cabsim);
	tt_sbuf_delete(tt->tmpbuf);
}

tt_generic_new(tintamp);
tt_generic_delete(tintamp);

void tt_tintamp_process(tt_tintamp_t *tt, tt_sbuf_t *inbuf, tt_sbuf_t *outbuf)
{
	tt_preamp_process(&tt->preamp, inbuf, tt->tmpbuf);
	tt_cabsim_process(&tt->cabsim, tt->tmpbuf, outbuf);
}
