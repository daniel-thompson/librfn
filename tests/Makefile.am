AM_CPPFLAGS = -I../src/librfn/include -I../src/libtt/include
AM_LDADD = ../src/libtt/libtt.a ../src/librfn/librfn.a
TESTS_ENVIRONMENT=$(VALGRIND)

noinst_PROGRAMS=\
	fuzztest \
	sampletest \
	analysetest \
	siggentest \
	biquadtest \
	waveshapertest \
	tubestagetest \
	preamptest \
	cabsimtest \
	tintamptest

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

waveshapertest_SOURCES=waveshapertest.c
waveshapertest_LDADD = $(AM_LDADD)

tubestagetest_SOURCES=tubestagetest.c
tubestagetest_LDADD = $(AM_LDADD)

preamptest_SOURCES=preamptest.c
preamptest_LDADD = $(AM_LDADD)

cabsimtest_SOURCES=cabsimtest.c
cabsimtest_LDADD = $(AM_LDADD)

tintamptest_SOURCES=tintamptest.c
tintamptest_LDADD = $(AM_LDADD)

TESTS = $(noinst_PROGRAMS)
