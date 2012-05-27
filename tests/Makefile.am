AM_CPPFLAGS = -DHAVE_FPU -I../src/librfn/include -I../src/libtt/include
AM_LDADD = ../src/libtt/libtt.a ../src/librfn/librfn.a

bin_PROGRAMS=biquadtest
biquadtest_SOURCES=biquadtest.c
biquadtest_LDADD = $(AM_LDADD)

TESTS = $(bin_PROGRAMS)