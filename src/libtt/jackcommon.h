/*
 * jackcommon.h
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

#ifndef TT_JACKCOMMON_H_
#define TT_JACKCOMMON_H_

#include <stdbool.h>

#include <jack/jack.h>

#include "libtt/sample.h"
#include "libtt/util.h"

jack_port_t *tt_jackcommon_new_port(tt_context_t *ctx, bool is_input);
void tt_jackcommon_delete_port(jack_port_t *port);

void tt_jackcommon_activate();
void tt_jackcommon_deactivate();

unsigned int tt_jackcommon_cycle_wait();
void tt_jackcommon_cycle_signal();




#endif // TT_ALSAWRITER_H_
