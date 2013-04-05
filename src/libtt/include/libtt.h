/*
 * libtt.h
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

#ifndef TT_LIBTT_H_
#define TT_LIBTT_H_

// fundamentals
#include "libtt/sbuf.h"
#include "libtt/util.h"

// building blocks
#ifdef HAVE_ALSA
#include "libtt/alsawriter.h"
#endif
#include "libtt/analyse.h"
#include "libtt/audiowriter.h"
#include "libtt/biquad.h"
#include "libtt/dcblocker.h"
#include "libtt/delay.h"
#include "libtt/firstorder.h"
#ifdef HAVE_PULSE
#include "libtt/pulsewriter.h"
#endif
#include "libtt/siggen.h"
#include "libtt/quantizer.h"
#include "libtt/reverb.h"
#include "libtt/waveshaper.h"
#include "libtt/wavreader.h"
#include "libtt/wavwriter.h"

// high level amp processing
#include "libtt/tubestage.h"
#include "libtt/preamp.h"
#include "libtt/tonestack.h"
#include "libtt/cabsim.h"
#include "libtt/tintamp.h"

// high level utilities
#include "libtt/drummachine.h"
#include "libtt/preset.h"
#include "libtt/presetio.h"

#endif // TT_LIBTT_H_
