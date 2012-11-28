bin_PROGRAMS=jacklab

jacklab_SOURCES=jacklab.c
jacklab_CPPFLAGS=$(JACK_CFLAGS) -I$(top_srcdir)/src/libtt/include -I$(top_srcdir)/src/librfn/include
jacklab_LDADD=$(JACK_LIBS) $(top_builddir)/src/libtt/libtt.a $(top_builddir)/src/librfn/librfn.a
