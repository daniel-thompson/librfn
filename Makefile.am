#
# Makefile.am
#
# Part of tintamp (the integer amplifier)
#
# Copyright (C) 2012, 2013 Daniel Thompson <daniel@redfelineninja.org.uk> 
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#


ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = \
	src/jacklab/jacklab \
	src/jacklua/tintamp_jacklua \
	$(src_tintamp_programs) \
	$(src_tintdrum_programs)
if GTK
src_tintamp_programs = src/tintamp/tintamp
endif
if HAVE_ALSA
src_tintdrum_programs = src/tintdrum/tintdrum
endif
if HAVE_PULSE
src_tintdrum_programs = src/tintdrum/tintdrum
endif

noinst_LIBRARIES =
noinst_PROGRAMS =
BUILT_SOURCES =
CLEANFILES =
TESTS =
TESTS_ENVIRONMENT=$(VALGRIND)

include Makefile-librfn.am
include Makefile-libtt.am


#
# jacklab
#

src_jacklab_jacklab_SOURCES = \
	src/jacklab/jacklab.c
src_jacklab_jacklab_CPPFLAGS = $(JACK_CFLAGS) $(LIBTT_CFLAGS)
src_jacklab_jacklab_LDADD = $(JACK_LIBS) $(LIBTT_LIBS)


#
# tintamp_jacklua
#

src_jacklua_tintamp_jacklua_SOURCES = \
	src/jacklua/jacklua.c
src_jacklua_tintamp_jacklua_CPPFLAGS = $(JACK_CFLAGS) $(LIBTT_CFLAGS)
src_jacklua_tintamp_jacklua_LDADD=$(JACK_LIBS) $(LIBTT_LIBS)


#
# tintamp
#

src_tintamp_tintamp_SOURCES = \
	src/tintamp/tintamp_gresource.c \
	src/tintamp/tintamp_main.c \
	src/tintamp/tintamp_ui.c
src_tintamp_tintamp_CPPFLAGS = $(JACK_CFLAGS) $(GTK_CFLAGS) $(LIBTT_CFLAGS)
src_tintamp_tintamp_LDADD = $(JACK_LIBS) $(GTK_LIBS) $(LIBTT_LIBS)

CLEANFILES += src/tintamp/tintamp_gresource.c
src/tintamp/tintamp_gresource.c : $(top_srcdir)/src/tintamp/tintamp.gresource.xml $(top_srcdir)/src/tintamp/tintamp.ui
	@echo "  COMPILE-RESOURCES $@"
	@glib-compile-resources \
		--generate-source \
		--sourcedir=$(top_srcdir)/src/tintamp \
		--target $@ \
		$(top_srcdir)/src/tintamp/tintamp.gresource.xml


#
# tintdrum
#

src_tintdrum_tintdrum_SOURCES = \
	src/tintdrum/tintdrum_main.c
src_tintdrum_tintdrum_CPPFLAGS = $(LIBTT_CFLAGS)
src_tintdrum_tintdrum_LDADD = $(LIBTT_LIBS) $(ALSA_LIBS) $(JACK_LIBS) $(PULSE_LIBS) -lpthread
