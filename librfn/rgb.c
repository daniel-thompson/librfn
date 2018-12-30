/*
 * rgb.c
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2019 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#include "librfn/rgb.h"

void rgb_fader_init(rgb_fader_t *f, uint32_t from, uint32_t to, int nsteps)
{
	uint32_t step = ((int32_t) (to - from) / nsteps);

	if (0 == step)
		step = 1;

	f->goal = to;
	f->step = step;
}
		
bool rgb_fade(rgb_fader_t *f, uint32_t *val)
{
	uint32_t from = *val;
	uint32_t to = from + f->step;

	/* if we overflow or overshoot the goal then directly adopt the goal */
	if (((from ^ to) & 0x80000000) ||
	    (f->step > 0 && to >= f->goal) ||
	    (f->step < 0 && to <= f->goal)) {
		*val = f->goal;
		return true;
	}

	*val = to;
	return false;
}

const rgb_gamma_t rgb_gamma_default = { 
        {
                  0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
                  1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,
                  2,   2,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,
                  4,   4,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,
                  7,   8,   8,   8,   8,   9,   9,   9,  10,  10,  10,  11,
                 11,  12,  12,  12,  13,  13,  14,  14,  15,  15,  15,  16,
                 16,  17,  17,  18,  18,  19,  19,  20,  20,  21,  22,  22,
                 23,  23,  24,  24,  25,  26,  26,  27,  28,  28,  29,  30,
                 30,  31,  32,  32,  33,  34,  34,  35,  36,  37,  37,  38,
                 39,  40,  40,  41,  42,  43,  44,  45,  45,  46,  47,  48,
                 49,  50,  51,  52,  53,  53,  54,  55,  56,  57,  58,  59,
                 60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  71,  72,
                 73,  74,  75,  76,  77,  78,  80,  81,  82,  83,  84,  85,
                 87,  88,  89,  90,  92,  93,  94,  95,  97,  98,  99, 101,
                102, 103, 105, 106, 107, 109, 110, 112, 113, 114, 116, 117,
                119, 120, 122, 123, 124, 126, 127, 129, 130, 132, 134, 135,
                137, 138, 140, 141, 143, 145, 146, 148, 149, 151, 153, 154,
                156, 158, 160, 161, 163, 165, 166, 168, 170, 172, 173, 175,
                177, 179, 181, 182, 184, 186, 188, 190, 192, 194, 196, 197,
                199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
                223, 225, 227, 229, 231, 234, 236, 238, 240, 242, 244, 246,
                248, 251, 253, 255
        }
};

uint32_t rgb_correct(const rgb_gamma_t *g, uint32_t val)
{
	return g->table[(uint8_t) val] |
	       g->table[(uint8_t) (val >> 8)] << 8 |
	       g->table[(uint8_t) (val >> 16)] << 16;
}
