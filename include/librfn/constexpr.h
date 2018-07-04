/*
 * constexpr.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2018 Daniel Thompson <daniel@redfelineninja.org.uk> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_CONSTEXPR_H_
#define RF_CONSTEXPR_H_

/*!
 * \defgroup librfn_constexpr Constant expressions
 *
 * \brief Utility macros based on constant expressions
 *
 * Constant expressions are a (very) limited sub-set of the C
 * grammar that can be used in initializers. These macros can be
 * used to implement a variety of exotic and/or over-complex 
 * initializers.
 *
 * \note If the arguments passed to the macros are not constant
 *       expressions then, instead of collapsing to a simple constant
 *       load, they will expand into extremely time and space
 *       inefficient inlined code.
 *
 * @{
 */

/*! \private */
#define const_pop1(c)  (c & 1)
/*! \private */
#define const_pop2(c)  (const_pop1(c) + const_pop1(c >> 1))
/*! \private */
#define const_pop4(c)  (const_pop2(c) + const_pop2(c >> 2))
/*! \private */
#define const_pop8(c)  (const_pop4(c) + const_pop4(c >> 4))
/*! \private */
#define const_pop16(c) (const_pop8(c) + const_pop8(c >> 8))
/*! \private */
#define const_pop32(c) (const_pop16(c) + const_pop16(c >> 16))
/*! \private */
#define const_pop64(c) (const_pop32(c) + const_pop32(c >> 32))


/*!
 * Count the number of set bits in the argument, c.
 */
#define const_pop(c) (const_pop64((uint64_t) (c)))

/*! \private */
#define const_lssb1(c)  (0x1 & c ? 0 : -64)
/*! \private */
#define const_lssb2(c)  (0x1 & c ? const_lssb1(c) : const_lssb1(c >> 1) + 1)
/*! \private */
#define const_lssb4(c)  (0x3 & c ? const_lssb2(c) : const_lssb2(c >> 2) + 2)
/*! \private */
#define const_lssb8(c)  (0xf & c ? const_lssb4(c) : const_lssb4(c >> 4) + 4)
/*! \private */
#define const_lssb16(c) (0xff & c ? const_lssb8(c) : const_lssb8(c >> 8) + 8)
/*! \private */
#define const_lssb32(c) (0xffff & c ? const_lssb16(c) : \
		         const_lssb16(c >> 16) + 16)
/*! \private */
#define const_lssb64(c) (0xffffffffull & c ? const_lssb32(c) : \
		         const_lssb32(c >> 32) + 32)

/*!
 * Find the least significant set bit in the argument, c.
 *
 * Also known as count trailing zeros (ctz).
 */
#define const_lssb(c) (const_lssb64((uint64_t) (c)))

/*! @} */
#endif /* RF_CONSTEXPR_H_ */
