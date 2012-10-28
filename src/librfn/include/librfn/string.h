/*
 * string.h
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

#ifndef RF_STRING_H_
#define RF_STRING_H_

#include <stdarg.h>
#include <stdlib.h>

char *xstrdup(const char *s);

char *strdup_join(const char *head, const char *tail);
char *xstrdup_join(const char *head, const char *tail);

char *strtolower(char *s);
char *strdup_tolower(const char *s);
char *xstrdup_tolower(const char *s);

char *strtoupper(char *s);
char *strdup_toupper(const char *s);
char *xstrdup_toupper(const char *s);


#endif // RF_UTIL_H_
