/*
 * reverb.c
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

#include "libtt.h"
#include "libtt/reverb.h"

void tt_reverb_init(tt_reverb_t *p, tt_context_t *ctx)
{
	memset(p, 0, sizeof(*p));
	p->ctx = ctx;

	// The delay lengths come from jcrev by John Chowning
	tt_delay_init(&p->allpass[0].delay, ctx, p->allpass347, 347);
	tt_delay_init(&p->allpass[1].delay, ctx, p->allpass113, 113);
	tt_delay_init(&p->allpass[2].delay, ctx, p->allpass37, 37);
	tt_delay_init(&p->comb[0].delay, ctx, p->comb1601, 1601);
	tt_delay_init(&p->comb[1].delay, ctx, p->comb1867, 1867);
	tt_delay_init(&p->comb[2].delay, ctx, p->comb2053, 2053);
	tt_delay_init(&p->comb[3].delay, ctx, p->comb2251, 2251);
}

void tt_reverb_finalize(tt_reverb_t *p)
{
}

tt_generic_new(reverb);
tt_generic_delete(reverb);

void tt_reverb_setup(tt_reverb_t *p)
{
	p->pregain = TTFLOAT(0.1);

	ttspl_t allpassc[] = { TTFLOAT(0.7), TTFLOAT(0.7), TTFLOAT(0.7) };
	for (int i=0; i<lengthof(p->allpass); i++) {
		tt_delay_setup(&p->allpass[i].delay);
		p->allpass[i].gainb = allpassc[i];
		p->allpass[i].gainf = TTSUB(TTINT(0), allpassc[i]);
	}

	// The comb tunings come from jcrev by John Chowning
	ttspl_t combb[] = { TTFLOAT(0.802), TTFLOAT(0.773), TTFLOAT(0.753), TTFLOAT(0.733) };
	for (int i=0; i<lengthof(p->comb); i++) {
		tt_delay_setup(&p->comb[i].delay);
		p->comb[i].gainb = combb[i];
	}
}

tt_generic_get_control(reverb, TT_REVERB_CONTROL_MIN, TT_REVERB_CONTROL_MAX);

void tt_reverb_set_control(tt_reverb_t *p, tt_reverb_control_t ctrl, ttspl_t val)
{
	assert(ctrl >= TT_REVERB_CONTROL_MIN && ctrl < TT_REVERB_CONTROL_MAX);
	p->controls[TT_TAG2ID(ctrl)] = val;

	// apply the change
}

tt_generic_enum_control(reverb, TT_REVERB_CONTROL_MIN, TT_REVERB_CONTROL_MAX);


ttspl_t tt_reverb_step(tt_reverb_t *p, ttspl_t spl)
{
	ttspl_t wet = TTMAL(spl, p->pregain);

	for (int i=0; i<lengthof(p->allpass); i++) {
		ttspl_t delayout = tt_delay_peek(&p->allpass[i].delay);
		ttspl_t feedback = TTMAL(delayout, p->allpass[i].gainb);

		ttspl_t delayin = TTADD(wet, feedback);
		(void) tt_delay_step(&p->allpass[i].delay, delayin);

		wet = TTADD(TTMAL(delayin, p->allpass[i].gainf), delayout);
	}

	ttspl_t comb_sum = TTINT(0);
	for (int i=0; i<lengthof(p->comb); i++) {
		ttspl_t delayout = tt_delay_peek(&p->comb[i].delay);
		ttspl_t feedback = TTMAL(delayout, p->comb[i].gainb);

		ttspl_t comb_out = TTADD(wet, feedback);
		(void) tt_delay_step(&p->comb[i].delay, comb_out);

		comb_sum = TTADD(comb_sum, comb_out);
	}

	wet = comb_sum;
	//ttspl_t seperation = TTMAL(wet, TTINT(-2));

	return TTADD(spl, wet);
}

tt_generic_process(reverb);
