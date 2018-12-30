/*
 * rgb.h
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

#ifndef RF_RGB_H_
#define RF_RGB_H_

#include <stdbool.h>
#include <stdint.h>

/*!
 * \defgroup librfn_rgb RGB colour manipulation
 *
 * \brief Utility functions to handle simple RGB effects such as 
 *        crossfade and agamma correction.
 *
 * @{
 */

/*!
 * \brief RGB fader state tracking.
 */
typedef struct rgb_fader {
	uint32_t goal;
	uint32_t step;
} rgb_fader_t;

/*!
 * \brief Start a cross fade.
 */
void rgb_fader_init(rgb_fader_t *f, uint32_t from, uint32_t to, int nsteps);
		
/*!
 * \brief Perform a step towards the new display
 *
 * \returns true if the fade has reached the final value
 */
bool rgb_fade(rgb_fader_t *f, uint32_t *val);

/*!
 * \brief Gamma correction table and related tuning
 */
typedef struct rgb_gamma {
	uint8_t table[256];
} rgb_gamma_t;

/*!
 * \brief A "sane default" correction table
 */
extern const rgb_gamma_t rgb_gamma_default;

/*!
 * \brief Look up a value on the gamma curve.
 */
uint32_t rgb_correct(const rgb_gamma_t *g, uint32_t val);


/*! @} */
#endif // RF_RGB_H_
