/*
 * benchmark.h
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

#ifndef RF_BENCHMARK_H_
#define RF_BENCHMARK_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	uint64_t start;
	uint64_t expiry;
	uint64_t end;
} rf_benchmark_t;

typedef struct {
	double ratio; //!< Degree by which we run faster than real time (x2 mean twice)
	double cpu_usage; //!< Estimated CPU usage at x1
} rf_benchmark_results_t;

/*!
 * Get the current wall time (in microseconds).
 *
 * This is merely a wrapper around clock_gettime() but less cumbersome to work with.
 */
uint64_t rf_benchmark_time_now();

void rf_benchmark_init(rf_benchmark_t *b, uint64_t runtime);
bool rf_benchmark_running(rf_benchmark_t *b);
void rf_benchmark_finalize(rf_benchmark_t *b, uint64_t nominal, rf_benchmark_results_t *r);
void rf_benchmark_results_show(rf_benchmark_results_t *r, const char *tag);

#endif // RF_BENCHMARK_H_
