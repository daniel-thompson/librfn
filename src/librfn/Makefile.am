noinst_LIBRARIES=librfn.a
librfn_a_CPPFLAGS=-I$(srcdir)/include
librfn_a_SOURCES=$(benchmark_SOURCES) bitops.c enum.c fuzz.c hex.c pack.c string.c wavheader.c util.c

if HAVE_CLOCK_GETTIME
benchmark_SOURCES = benchmark.c
endif
