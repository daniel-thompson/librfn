if GTK
bin_PROGRAMS=tintamp
endif

tintamp_SOURCES=tintamp_gresource.c tintamp_main.c tintamp_ui.c
tintamp_CPPFLAGS=$(JACK_CFLAGS) $(GTK_CFLAGS) -I$(top_srcdir)/src/libtt/include -I$(top_srcdir)/src/librfn/include
tintamp_LDADD=$(JACK_LIBS) $(GTK_LIBS) $(top_builddir)/src/libtt/libtt.a $(top_builddir)/src/librfn/librfn.a

CLEANFILES = tintamp_gresource.c
tintamp_gresource.c : $(srcdir)/tintamp.gresource.xml $(srcdir)/tintamp.ui
	@echo "  COMPILE-RESOURCES tintamp.gresource.xml"
	@glib-compile-resources \
		--generate-source \
		--sourcedir=$(srcdir) \
		--target $@ \
		$(srcdir)/tintamp.gresource.xml
