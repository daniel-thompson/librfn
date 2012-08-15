/*
 * sample.h
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

#ifndef TT_SAMPLE_H_
#define TT_SAMPLE_H_

#include <stdint.h>
#include <math.h>

//
// The fixed point limits are useful even when operating in floating point
// mode because that way the runtime asserts are still meaningful.
//

#define TTQ 20			//!< Fractional bits for normal precision (Q19)
#define TTSPL_MAX ((1 << (31-TTQ)) - 1)
#define TTSPL_MIN (-TTSPL_MAX)

#define TLQ (2*TTQ)		//!< Fractional bits for "long" precision (Q38)
#define TLSPL_MAX ((1ll << (63-TLQ)) - 1)
#define TLSPL_MIN (-TLSPL_MAX)

#define TSQ 15			//!< Fractional bits for "short" precision (Q15)


#ifdef HAVE_FPU

typedef float ttspl_t;				//!< Sample in normal precision samples
#define TTVALID(x) (isfinite(x) && (TTSPL_MIN <= (x)) && ((x) < TTSPL_MAX))

typedef float tlspl_t;
#define TLVALID(x) (isfinite(x) && (TLSPL_MIN <= (x)) && ((x) < TLSPL_MAX))

#ifdef ENABLE_RANGE_CHECK
ttspl_t ttvalidate(ttspl_t spl, const char *fname, int lineno);
#define TTVALIDATE(x) ttvalidate(x, __FILE__, __LINE__)
tlspl_t tlvalidate(tlspl_t spl, const char *fname, int lineno);
#define TLVALIDATE(x) tlvalidate(x, __FILE__, __LINE__)
#else
#define TTVALIDATE(x) (x)
#define TLVALIDATE(x) (x)
#endif

#define TTFLOAT(a) (a)               		//!< Conversion from float
#define TTASFLOAT(a) (a)              		//!< Conversion to float
#define TTINT(a) ((ttspl_t) (a))		//!< Conversion from integer
#define TTASINT(a) ((uint32_t) (a))		//!< Conversion to integer
#define TTS16LE(a) ((a) / 32767.0)		//!< Conversion from S16LE
#define TTASS16LE(a) ((a) * 32767.0)		//!< Conversion to S16LE
#define TTS32LE(a) ((a) / 2147483647.0)		//!< Conversion from S32LE
#define TTASS32LE(a) ((a) * 2147483647.0)	//!< Conversion to S32LE

#define TTRAISE(x) TLVALIDATE(TTVALIDATE(x))				//!< Conversion to long precision

#define TTADD(a, b) TTVALIDATE(TTVALIDATE(a) + TTVALIDATE(b))			//!< Add operation (normal precision)
#define TTSUB(a, b) TTVALIDATE(TTVALIDATE(a) - TTVALIDATE(b))			//!< Subtract operation (normal precision)
#define TTNEGATE(x) TTVALIDATE(-TTVALIDATE(x))			//!< Negate operation (normal precision)
#define TTMUL(a, b) TLVALIDATE(TTVALIDATE(a) * TTVALIDATE(b))			//!< Multiply \returns Long precision
#define TTMULI(a, b) TLVALIDATE(TTVALIDATE(a) * (b))			//!< Multiply by integer \returns Long precision
#define TTMAL(a, b) TTVALIDATE(TTVALIDATE(a) * TTVALIDATE(b))			//!< Multiply and lower \returns Normal precision
#define TTMINT(a, b) TTVALIDATE(TTVALIDATE(a) * (b))		//!< Multiply by integer
#define TTDIV(a, b) TTVALIDATE(TTVALIDATE(a) / TTVALIDATE(b))			//!< Divide. Will overflow if a is large.
#define TTRAD(a, b) TTVALIDATE(TTVALIDATE(a) / TTVALIDATE(b))			//!< Raise and divide.
#define TTDINT(a, b) TTVALIDATE(TTVALIDATE(a) / (b))		//!< Divide by integer

#define TTADDI(a, b) TTVALIDATE(TTVALIDATE(a) + (b))
#define TTSUBI(a, b) TTVALIDATE(TTVALIDATE(a) - (b))
#define TTISUB(a, b) TTVALIDATE((a) - TTVALIDATE(b))

#define TTMAC(acc, a, b) TLVALIDATE((acc) += TTVALIDATE(a) * TTVALIDATE(b))	//!< Multiply and accumulate (acc is long precision)
#define TTMACI(acc, a, b) TLVALIDATE((acc) += TTVALIDATE(a) * (b))	//!< Multiple by integer and accumulate

#define TTPI ((ttspl_t) (M_PI))

#define TTABS(x) TTVALIDATE(fabsf(TTVALIDATE(x)))
#define TTCOS(x) TTVALIDATE(cosf(TTVALIDATE(x)))
#define TTEXP(x) TTVALIDATE(expf(TTVALIDATE(x)))
#define TTLOG2(x) TTVALIDATE(log2(TTVALIDATE(x)))
#define TTLOG10(x) TTVALIDATE(log10(TTVALIDATE(x)))
#define TTPOW(a, b) TTVALIDATE(powf(TTVALIDATE(a), TTVALIDATE(b)))
#define TTSIN(x) TTVALIDATE(sinf(TTVALIDATE(x)))
#define TTSQRT(x) TTVALIDATE(sqrtf(TTVALIDATE(x)))


#define TLFLOAT(a) (a)               		//!< Conversion from float
#define TLINT(a) ((ttspl_t) (a))		//!< Conversion from integer

#define TLLOWER(x) TTVALIDATE(TLVALIDATE(x))				//!< Conversion to normal precision

#define TLADD(a, b) TLVALIDATE(TLVALIDATE(a) + TLVALIDATE(b))			//!< Add operation (long precision)
#define TLSUB(a, b) TLVALIDATE(TLVALIDATE(a) - TLVALIDATE(b))			//!< Subtract operation (long precision)
#define TLMINT(a, b) TLVALIDATE(TLVALIDATE(a) * (b))
#define TLDIV(a, b) TTVALIDATE(TLVALIDATE(a) / TTVALIDATE(b))			//!< Divide. \arg b is normal precision
#define TLDINT(a, b) TLVALIDATE(TLVALIDATE(a) / (b))		//!< Divide long precision number by integer
						//   \return Long precision result

#else // HAVE_FPU

typedef int32_t ttspl_t;				//!< Sample in normal precision samples

#define TTFLOAT(a) ((ttspl_t) ((a) * (float) (1 << TTQ)))
#define TTASFLOAT(a) ((float) (a) / (float) (1 << TTQ))
#define TTINT(a) ((a) << TTQ)			//!< Conversion from integer
#define TTASINT(a) ((a) >> TTQ)			//!< Conversion to integer
#define TTS16LE(a) ((a) << (TTQ - 15))		//!< Conversion from S16LE
#define TTASS16LE(a) ((a) >> (TTQ - 15))	//!< Conversion to S16LE
#define TTS32LE(a) ((a) >> (31 - TTQ))		//!< Conversion from S32LE
#define TTASS32LE(a) ((a) << (31 - TTQ))	//!< Conversion to S32LE

#define TTRAISE(x) ((tlspl_t) (x) << (TLQ - TTQ)) //!< Conversion to long precision

#define TTADD(a, b) ((a) + (b))			//!< Add operation (normal precision)
#define TTSUB(a, b) ((a) - (b))			//!< Subtract operation (normal precision)
#define TTNEGATE(x) (-(x))			//!< Negate operation (normal precision)
#define TTMUL(a, b) ((tlspl_t) (a) * (tlspl_t) (b)) //!< Multiply \returns Long precision
#define TTMULI(a, b) (TTMUL((a), (b)) << (TLQ - TTQ))	//!< Multiple by integer \returns Long precision

#define TTMAL(a, b) TLLOWER(TTMUL((a), (b)))	//!< Multiply and lower \returns Normal precision
#define TTMINT(a, b) ((a) * (b))		//!< Multiply by integer
#define TTDIV(a, b) ((((a) << (31-TTQ)) / ((b) >> (TTQ-15))) << (TTQ + 15 - 31)) //!< Divide. Looses precision and will overflow if a is larger than one.
#define TTRAD(a, b) TLDIV(TTRAISE(a), (b))	//!< Raise and divide.
#define TTDINT(a, b) ((a) / (b))		//!< Divide by integer

#define TTADDI(a, b) ((a) + TTINT(b))
#define TTSUBI(a, b) ((a) - TTINT(b))
#define TTISUB(a, b) (TTINT(a) - (b))

#define TTMAC(acc, a, b) ((acc) += TTMUL((a), (b)))	//!< Multiply and accumulate (acc is long precision)
#define TTMACI(acc, a, b) ((acc) += TTMULI((a), (b)))	//!< Multiple by integer and accumulate

#define TTPI TTFLOAT(M_PI)

ttspl_t ttabs(ttspl_t x);
ttspl_t ttcos(ttspl_t x);
ttspl_t ttexp(ttspl_t x);
ttspl_t ttlog2(ttspl_t x);
ttspl_t ttlog10(ttspl_t x);
ttspl_t ttpow(ttspl_t a, ttspl_t b);
ttspl_t ttsin(ttspl_t x);
ttspl_t ttsqrt(ttspl_t x);

#define TTABS(x) ttabs(x)
#define TTCOS(x) ttcos(x)
#define TTEXP(x) ttexp(x)
#define TTLOG2(x) ttlog2(x)
#define TTLOG10(x) ttlog10(x)
#define TTPOW(a, b) ttpow(a, b)
#define TTSIN(x) ttsin(x)
#define TTSQRT(x) ttsqrt(x)

#define TTVALID(x) 1

typedef int64_t tlspl_t;

#define TLFLOAT(a) ((tlspl_t) ((a) * (float) (1ll << TLQ))) //!< Conversion from float
#define TLINT(a) ((tlspl_t) (a) << TLQ)		//!< Conversion from integer

#define TLLOWER(x) ((ttspl_t) (x >> (TLQ - TTQ))) //!< Conversion to normal precision
#define TLERROR(x) ((x) & ((1 << (TLQ-TTQ)) - 1)) //!< Lost precision due to truncation

#define TLADD(a, b) ((a) + (b))			//!< Add operation (long precision)
#define TLSUB(a, b) ((a) - (b))			//!< Subtract operation (long precision)
#define TLMINT(a, b) ((a) * (b))
#define TLDIV(a, b) ((a) / (b))			//!< Divide. \arg b is normal precision
#define TLDINT(a, b) ((a) / (b))		//!< Divide long precision number by integer
						//   \return Long precision result


#endif // HAVE_FPU

#define TTFRACTION(x) TTSUBI((x), TTASINT(x))

#define TTDB2LINEAR(x) TTPOW(TTINT(10), TTDINT((x), 20))
#define TTLINEAR2DB(x) TTMINT(TTLOG10(x), 20)

#endif // TT_SAMPLE_H_
