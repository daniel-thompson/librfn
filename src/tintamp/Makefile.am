bin_PROGRAMS=tintamp

tintamp_SOURCES=tintamp_gresource.c tintamp_main.c tintamp_ui.c
tintamp_CPPFLAGS=$(JACK_CFLAGS) $(GTK_CFLAGS) -I../libtt/include -I../librfn/include
tintamp_LDADD=$(JACK_LIBS) $(GTK_LIBS) ../libtt/libtt.a ../librfn/librfn.a

CLEANFILES = tintamp_gresource.c
tintamp_gresource.c : tintamp.gresource.xml tintamp.ui
	@echo "  COMPILE-RESOURCES tintamp.gresource.xml"
	@glib-compile-resources \
		--generate-source \
		--target $@ \
		tintamp.gresource.xml
