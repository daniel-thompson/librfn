noinst_LIBRARIES=libtt.a
libtt_a_CPPFLAGS=$(PULSE_CFLAGS) -I$(top_srcdir)/src/librfn/include -I$(srcdir)/include
libtt_a_SOURCES=\
	sample.c \
	sbuf.c \
	util.c \
	$(alsawriter_SOURCES) \
	analyse.c \
	audiowriter.c \
	biquad.c \
	dcblocker.c \
	firstorder.c \
	$(pulsewriter_SOURCES) \
	siggen.c \
	quantizer.c \
	waveshaper.c \
	wavreader.c \
	wavwriter.c \
	tubestage.c \
	preamp.c \
	tonestack.c \
	cabsim.c \
	tintamp.c \
	drummachine.c \
	preset.c \
	presetio.c

if HAVE_ALSA
alsawriter_SOURCES = alsawriter.c
endif
if HAVE_PULSE
pulsewriter_SOURCES = pulsewriter.c
endif
