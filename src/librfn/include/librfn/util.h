/*
 * util.h
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

#ifndef RF_UTIL_H_
#define RF_UTIL_H_

#include <stdarg.h>

char *strdup_printf(const char *format, ...);
char *strdup_vprintf(const char *format, va_list ap);

#endif // RF_UTIL_H_
