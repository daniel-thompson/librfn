/*
 * tintdrum.c
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

#include <string.h>

#include "librfn.h"
#include "libtt.h"

void tt_tintdrum_init(tt_tintdrum_t *tt, tt_context_t *ctx)
{
	memset(tt, 0, sizeof(*tt));
	tt->ctx = ctx;
	tt_drummachine_init(&tt->drummachine, ctx);
	tt_reverb_init(&tt->reverb, ctx);
	tt->tmpbuf = tt_sbuf_new(ctx->grain_size);

	tt_drummachine_setup(&tt->drummachine);
	tt_reverb_setup(&tt->reverb);
}

void tt_tintdrum_finalize(tt_tintdrum_t *tt)
{
	tt_reverb_finalize(&tt->reverb);
	tt_drummachine_finalize(&tt->drummachine);
	tt_sbuf_delete(tt->tmpbuf);
}

tt_generic_new(tintdrum);
tt_generic_delete(tintdrum);

ttspl_t tt_tintdrum_get_control(tt_tintdrum_t *tt, tt_tintdrum_control_t ctrl)
{
	switch(TT_TAG2BASE(ctrl)) {
	case TT_DRUMMACHINE_BASE:
		return tt_drummachine_get_control(&tt->drummachine, ctrl);
	case TT_REVERB_BASE:
		return tt_reverb_get_control(&tt->reverb, ctrl);
	case TT_TINTDRUM_BASE:
	default:
		break;
	}

	assert(ctrl >= TT_TINTDRUM_CONTROL_MIN && ctrl < TT_TINTDRUM_CONTROL_MAX);

	switch(ctrl) {
	case TT_TINTDRUM_CONTROL_MASTER_VOLUME:
		return TTINT(1);
	default:
		assert(0);
		break;
	}

	return TTINT(0);
}
void tt_tintdrum_set_control(tt_tintdrum_t *tt, tt_tintdrum_control_t ctrl, ttspl_t val)
{
	switch(TT_TAG2BASE(ctrl)) {
	case TT_DRUMMACHINE_BASE:
		tt_drummachine_set_control(&tt->drummachine, ctrl, val);
		return;
	case TT_REVERB_BASE:
		tt_reverb_set_control(&tt->reverb, ctrl, val);
		return;
	case TT_TINTDRUM_BASE:
	default:
		break;
	}

	assert(ctrl >= TT_TINTDRUM_CONTROL_MIN && ctrl < TT_TINTDRUM_CONTROL_MAX);

	switch(ctrl) {
	case TT_TINTDRUM_CONTROL_MASTER_VOLUME:
		return;
	default:
		assert(0);
		break;
	}

}

tt_tintdrum_control_t tt_tintdrum_enum_control(tt_tintdrum_control_t ctrl)
{
	if (0 == ctrl)
		return TT_PREAMP_CONTROL_MIN;

	switch(TT_TAG2BASE(ctrl)) {
	case TT_PREAMP_BASE:
		ctrl = tt_preamp_enum_control(ctrl);
		if (ctrl)
			return ctrl;
		//FALLTHRU
	case TT_TONESTACK_BASE:
		ctrl = tt_tonestack_enum_control(ctrl);
		// do not expose the tonestack gain control, we expose this as master volume
		if (TT_TONESTACK_CONTROL_GAIN == (int) ctrl)
			return tt_tintdrum_enum_control(ctrl);
		if (ctrl)
			return ctrl;
		//FALLTHRU
#if 0
	case TT_CABSIM_BASE:
		ctrl = tt_cabsim_enum_control(ctrl);
		if (ctrl)
			return ctrl;
		//FALLTHRU
#endif
	default:
		return (ctrl < TT_TINTDRUM_CONTROL_MIN  ? TT_TINTDRUM_CONTROL_MIN :
			ctrl >= TT_TINTDRUM_CONTROL_MAX-1 ? 0 :
				                           ctrl+1);
	}
}


void tt_tintdrum_process(tt_tintdrum_t *tt, tt_sbuf_t *outbuf)
{
	tt_drummachine_process(&tt->drummachine, tt->tmpbuf);
	tt_reverb_process(&tt->reverb, tt->tmpbuf, outbuf);
}
