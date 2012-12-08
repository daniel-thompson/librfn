/*
 * audiowriter.c
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

#include "libtt.h"
#include "libtt/audiowriter.h"

static void tt_audiowriter_null(void)
{
}

void tt_audiowriter_init(tt_audiowriter_t *p, tt_context_t *ctx)
{
	// when we add jack support this will become runtime/environment selected
	p->vtable = NULL;

#ifdef HAVE_PULSE
	if (NULL == p->vtable) {
		static tt_audiowriter_vtable_t pulse_vtable = {
			(void *) &tt_pulsewriter_finalize,
			(void *) &tt_pulsewriter_setup,
			(void *) &tt_pulsewriter_step,
			(void *) &tt_pulsewriter_process
		};

		tt_pulsewriter_init(&p->u.pulsewriter, ctx);
		p->vtable = &pulse_vtable;
	}
#endif

#ifdef HAVE_ALSA
	if (NULL == p->vtable) {
		static tt_audiowriter_vtable_t alsa_vtable = {
			(void *) &tt_alsawriter_finalize,
			(void *) &tt_alsawriter_setup,
			(void *) &tt_alsawriter_step,
			(void *) &tt_alsawriter_process

		};

		tt_alsawriter_init(&p->u.alsawriter, ctx);
		p->vtable = &alsa_vtable;
	}
#endif
	if (NULL == p->vtable) {
		static tt_audiowriter_vtable_t null_vtable = {
			(void *) &tt_audiowriter_null,
			(void *) &tt_audiowriter_null,
			(void *) &tt_audiowriter_null,
			(void *) &tt_audiowriter_null
		};

		p->vtable = &null_vtable;
	}
}

void tt_audiowriter_finalize(tt_audiowriter_t *p)
{
	p->vtable->finalize(p);
}

tt_generic_new(audiowriter);
tt_generic_delete(audiowriter);

void tt_audiowriter_setup(tt_audiowriter_t *p)
{
	p->vtable->setup(p);
}

void tt_audiowriter_step(tt_audiowriter_t *p, ttspl_t spl)
{
	p->vtable->step(p, spl);
}

void tt_audiowriter_process(tt_audiowriter_t *p, tt_sbuf_t *outbuf)
{
	p->vtable->process(p, outbuf);
}

