/*
 * enum.h
 *
 * Part of librfn (a general utility library from redfelineninja.org.uk)
 *
 * Copyright (C) 2012 Daniel Thompson <daniel@redfelineninja.org.uk> 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef RF_ENUM_H_
#define RF_ENUM_H_

typedef struct {
	const char *s;
	int e;
} rf_enumtable_t;

/*!
 * Initialize a enum lookup table.
 *
 * Using this macro optional but guarantees that the initializer is
 * correctly terminated.
 */
#define RF_ENUMTABLE_INITIALIZER(...) { __VA_ARGS__, RF_ENUMTABLE_TERMINATOR }

#define RF_ENUM_OUT_OF_RANGE -19830927
#define RF_ENUMTABLE_TERMINATOR { NULL, 0 }


const char *rf_enum2string(rf_enumtable_t *t, int e);
int rf_string2enum(rf_enumtable_t *t, const char *s);

#endif // RF_ENUM_H_
