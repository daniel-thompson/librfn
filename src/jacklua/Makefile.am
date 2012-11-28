bin_PROGRAMS=tintamp_jacklua

tintamp_jacklua_SOURCES=jacklua.c
tintamp_jacklua_CPPFLAGS=$(JACK_CFLAGS) -I$(top_srcdir)/src/libtt/include -I$(top_srcdir)/src/librfn/include
tintamp_jacklua_LDADD=$(JACK_LIBS) $(top_builddir)/src/libtt/libtt.a $(top_builddir)/src/librfn/librfn.a
