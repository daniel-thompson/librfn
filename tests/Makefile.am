AM_CPPFLAGS = -I../src/librfn/include -I../src/libtt/include
AM_LDADD = ../src/libtt/libtt.a ../src/librfn/librfn.a

bin_PROGRAMS=fuzztest sampletest analysetest siggentest biquadtest 

#
# librfn tests
#

fuzztest_SOURCES=fuzztest.c
fuzztest_LDADD = $(AM_LDADD)

#
# libtt tests
#

sampletest_SOURCES=sampletest.c
sampletest_LDADD = $(AM_LDADD)

analysetest_SOURCES=analysetest.c
analysetest_LDADD = $(AM_LDADD)

siggentest_SOURCES=siggentest.c
siggentest_LDADD = $(AM_LDADD)

biquadtest_SOURCES=biquadtest.c
biquadtest_LDADD = $(AM_LDADD)

TESTS = $(bin_PROGRAMS)
