/*
 * analyse.h
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

#ifndef TT_ANALYSE_H_
#define TT_ANALYSE_H_

#include <stdbool.h>

#include "sbuf.h"

ttspl_t tt_analyse_peak(tt_sbuf_t *sbuf);
ttspl_t tt_analyse_rectify(tt_sbuf_t *sbuf);
bool tt_analyse_valid(tt_sbuf_t *sbuf);


#endif // TT_ANALYSE_H_
