AM_CPPFLAGS = -I$(top_srcdir)/src/librfn/include -I$(top_srcdir)/src/libtt/include
AM_LDADD = $(top_builddir)/src/libtt/libtt.a $(top_builddir)/src/librfn/librfn.a
TESTS_ENVIRONMENT=$(VALGRIND)

noinst_PROGRAMS=\
	enumtest \
	fuzztest \
	wavheadertest \
	sampletest \
	analysetest \
	siggentest \
	biquadtest \
	waveshapertest \
	wavreadertest \
	tubestagetest \
	preamptest \
	tonestacktest \
	cabsimtest \
	tintamptest \
	drummachinetest \
	presettest \
	benchmark

#
# librfn tests
#

enumtest_SOURCES=enumtest.c
enumtest_LDADD = $(AM_LDADD)

fuzztest_SOURCES=fuzztest.c
fuzztest_LDADD = $(AM_LDADD)

wavheadertest_SOURCES=wavheadertest.c
wavheadertest_LDADD = $(AM_LDADD)

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

wavreadertest_SOURCES=wavreadertest.c
wavreadertest_LDADD = $(AM_LDADD)

tubestagetest_SOURCES=tubestagetest.c
tubestagetest_LDADD = $(AM_LDADD)

preamptest_SOURCES=preamptest.c
preamptest_LDADD = $(AM_LDADD)

tonestacktest_SOURCES=tonestacktest.c
tonestacktest_LDADD = $(AM_LDADD)

cabsimtest_SOURCES=cabsimtest.c
cabsimtest_LDADD = $(AM_LDADD)

tintamptest_SOURCES=tintamptest.c
tintamptest_LDADD = $(AM_LDADD)

drummachinetest_SOURCES=drummachinetest.c
drummachinetest_LDADD = $(AM_LDADD)

presettest_SOURCES=presettest.c
presettest_LDADD = $(AM_LDADD)

benchmark_SOURCES=benchmark.c
benchmark_LDADD = $(AM_LDADD)

TESTS = $(noinst_PROGRAMS)
