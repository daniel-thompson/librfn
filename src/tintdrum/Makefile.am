if HAVE_PULSE
bin_PROGRAMS=tintdrum
endif

tintdrum_SOURCES=tintdrum_main.c
tintdrum_CPPFLAGS=$(PULSE_CFLAGS) -I../libtt/include -I../librfn/include
tintdrum_LDADD=$(PULSE_LIBS) ../libtt/libtt.a ../librfn/librfn.a -lpulse-simple
