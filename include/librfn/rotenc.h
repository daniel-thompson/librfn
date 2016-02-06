/*
 * rotenc.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2016 Daniel Thompson <daniel@redfelineninja.org.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_ROTENC_H_
#define RF_ROTENC_H_

#include <stdint.h>

/*!
 * \defgroup librfn_rotenc Rotary encoder
 *
 * \brief Utility functions to decode signals from a 2-bit rotary encoder.
 *
 * @{
 */

/*!
 * \brief Rotary encoder click counts and state tracking.
 */
typedef struct rotenc {
	uint8_t last_state;
	uint8_t count;
	uint16_t internal_count;
} rotenc_t;

/*!
 * \brief Static initializer for a rotary encoder.
 */
#define ROTENC_VAR_INIT { 0 }

/*!
 * \brief Handle a state update.
 *
 * It is safe to call this function when there has not actually been a
 * change of state, this allows it to be used as a polling function.
 *
 * No external debounce is required.
 */
void rotenc_decode(rotenc_t *r, uint8_t state);


/*!
 * \brief Read a 14-bit count of the current knob position.
 *
 * Suitable for higher resolution devices.
 */
uint16_t rotenc_count14(rotenc_t *r);

/*!
 * \brief Read an 8-bit count of the current knob position.
 *
 * Often 8-bit counts are sufficient (>10 turns at 24 clicks per turn).
 */
static inline uint8_t rotenc_count(rotenc_t *r)
{
	return r->count;
}

/*! @} */
#endif // RF_ROTENC_H_
