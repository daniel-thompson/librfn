if HAVE_ALSA
bin_PROGRAMS=tintdrum
endif
if HAVE_PULSE
bin_PROGRAMS=tintdrum
endif

tintdrum_SOURCES=tintdrum_main.c
tintdrum_CPPFLAGS=$(PULSE_CFLAGS) -I$(top_srcdir)/src/libtt/include -I$(top_srcdir)/src/librfn/include
tintdrum_LDADD=$(top_builddir)/src/libtt/libtt.a $(top_builddir)/src/librfn/librfn.a $(ALSA_LIBS) $(PULSE_LIBS)
