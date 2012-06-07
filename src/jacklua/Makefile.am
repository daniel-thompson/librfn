bin_PROGRAMS=tintamp_jacklua

tintamp_jacklua_SOURCES=jacklua.c
tintamp_jacklua_CPPFLAGS=$(jack_CFLAGS) -I../libtt/include -I../librfn/include
tintamp_jacklua_LDADD=$(jack_LIBS) ../libtt/libtt.a ../librfn/librfn.a
