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

#define TTQ 19			//!< Fractional bits for normal precision (Q19)
#define TTSPL_MAX ((1 << (32-TTQ)) - 1)
#define TTSPL_MIN (-TTSPL_MAX)

#define TLQ 38			//!< Fractional bits for "long" precision (Q38)
#define TLSPL_MAX ((1 << (32-TTL)) - 1)
#define TLSPL_MIN (-TLSPL_MAX)

#define TSQ 15			//!< Fractional bits for "short" precision (Q15)


#ifdef HAVE_FPU

typedef float ttspl_t;				//!< Sample in normal precision samples

#define TTFLOAT(a) (a)               		//!< Conversion from float
#define TTASFLOAT(a) (a)              		//!< Conversion to float
#define TTINT(a) ((ttspl_t) (a))		//!< Conversion from integer
#define TTASINT(a) ((uint32_t) (a))		//!< Conversion to integer
#define TTS16LE(a) ((a) / 32767.0)		//!< Conversion from S16LE
#define TTASS16LE(a) ((a) * 32767.0)		//!< Conversion to S16LE
#define TTS32LE(a) ((a) / 2147483647.0)		//!< Conversion from S32LE
#define TTASS32LE(a) ((a) * 2147483647.0)	//!< Conversion to S32LE

#define TTRAISE(x) (x)				//!< Conversion to long precision

#define TTADD(a, b) ((a) + (b))			//!< Add operation (normal precision)
#define TTSUB(a, b) ((a) - (b))			//!< Subtract operation (normal precision)
#define TTNEGATE(x) (-(x))			//!< Negate operation (normal precision)
#define TTMUL(a, b) ((a) * (b))			//!< Multiply \returns Long precision
#define TTMAL(a, b) ((a) * (b))			//!< Multiply and lower \returns Normal precision
#define TTMINT(a, b) ((a) * (b))		//!< Multiply by integer
#define TTDIV(a, b) ((a) / (b))			//!< Divide. Will overflow if a is large.
#define TTRAD(a, b) ((a) / (b))			//!< Raise and divide.
#define TTDINT(a, b) ((a) / (b))		//!< Divide by integer

#define TTADDI(a, b) ((a) + (b))
#define TTSUBI(a, b) ((a) - (b))
#define TTISUB(a, b) ((a) - (b))


#define TTMAC(acc, a, b) ((acc) += (a) * (b))	//!< Multiply and accumulate (acc is long precision)

#define TTPI ((ttspl_t) (M_PI))

#define TTABS(x) fabsf(x)
#define TTCOS(x) cosf(x)
#define TTLOG2(x) log2(x)
#define TTLOG10(x) log10(x)
#define TTPOW(a, b) powf(a, b)
#define TTSIN(x) sinf(x)
#define TTSQRT(x) sqrtf(x)

#define TTVALID(x) isfinite(x)
typedef float tlspl_t;

#define TLFLOAT(a) (a)               		//!< Conversion from float
#define TLINT(a) ((ttspl_t) (a))		//!< Conversion from integer

#define TLLOWER(x) (x)				//!< Conversion to normal precision

#define TLADD(a, b) ((a) + (b))			//!< Add operation (long precision)
#define TLSUB(a, b) ((a) - (b))			//!< Subtract operation (long precision)
#define TLDIV(a, b) ((a) / (b))			//!< Divide. \arg b is normal precision
#define TLDINT(a, b) ((a) / (b))		//!< Divide long precision number by integer
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
#define TTMAL(a, b) TLLOWER(TTMUL((a), (b)))	//!< Multiply and lower \returns Normal precision
#define TTMINT(a, b) ((a) * (b))		//!< Multiply by integer
#define TTDIV(a, b) ((((a) << (31-TTQ)) / ((b) >> (TTQ-15))) << (TTQ + 15 - 31)) //!< Divide. Looses precision and will overflow if a is larger than one.
#define TTRAD(a, b) TLDIV(TTRAISE(a), (b))	//!< Raise and divide.
#define TTDINT(a, b) ((a) / (b))		//!< Divide by integer

#define TTADDI(a, b) ((a) + TTINT(b))
#define TTSUBI(a, b) ((a) - TTINT(b))
#define TTISUB(a, b) (TTINT(a) - (b))


#define TTMAC(acc, a, b) ((acc) += TTMUL((a), (b)))	//!< Multiply and accumulate (acc is long precision)

#define TTPI TTFLOAT(M_PI)

#define TTABS(x) TTFLOAT(fabsf(TTASFLOAT(x)))
#define TTCOS(x) TTFLOAT(cosf(TTASFLOAT(x)))
#define TTLOG2(x) TTFLOAT(log2(TTASFLOAT(x)))
#define TTLOG10(x) TTFLOAT(log10(TTASFLOAT(x)))
#define TTPOW(a, b) TTFLOAT(powf(TTASFLOAT(a), TTASFLOAT(b)))
#define TTSIN(x) TTFLOAT(sinf(TTASFLOAT(x)))
#define TTSQRT(x) TTFLOAT(sqrtf(TTASFLOAT(x)))

#define TTVALID(x) 1

typedef int64_t tlspl_t;

#define TLFLOAT(a) ((tlspl_t) ((a) * (float) (1ll << TLQ))) //!< Conversion from float
#define TLINT(a) ((tlspl_t) (a) << TLQ)		//!< Conversion from integer

#define TLLOWER(x) ((ttspl_t) (x >> (TLQ - TTQ))) //!< Conversion to normal precision
#define TLERROR(x) 0

#define TLADD(a, b) ((a) + (b))			//!< Add operation (long precision)
#define TLSUB(a, b) ((a) - (b))			//!< Subtract operation (long precision)
#define TLDIV(a, b) ((a) / (b))			//!< Divide. \arg b is normal precision
#define TLDINT(a, b) ((a) / (b))		//!< Divide long precision number by integer
						//   \return Long precision result


#endif // HAVE_FPU

#define TTFRACTION(x) TTSUB((x), TTINT(TTASINT(x))) 

#define TTDB2LINEAR(x) TTPOW(TTINT(10), TTDINT((x), 20))
#define TTLINEAR2DB(x) TTMINT(TTLOG10(x), 20)

#endif // TT_SAMPLE_H_
