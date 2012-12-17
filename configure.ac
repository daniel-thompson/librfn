dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(tintamp, 1.0)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([1.11 silent-rules foreign -Wall])
AM_SILENT_RULES([yes])

AC_PROG_CC_C99
AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], [AC_MSG_ERROR([compiler does not support C99])])
AM_PROG_CC_C_O
AC_PROG_RANLIB
gl_VALGRIND_TESTS
AC_SEARCH_LIBS([cos], [m])
AC_SEARCH_LIBS([clock_gettime], [rt],
	AM_CONDITIONAL(HAVE_CLOCK_GETTIME, true)
          AC_DEFINE(HAVE_CLOCK_GETTIME,1,[Have clock_gettime]),
	AM_CONDITIONAL(HAVE_CLOCK_GETTIME, false))

dnl Benchmarking is off by default (because it takes ages in valgrind).
AC_ARG_ENABLE([benchmark],
    AS_HELP_STRING([--enable-benchmark], [Enable benchmark test (long runtime under valgrind)]))
AM_CONDITIONAL(BENCHMARK, [test "x$enable_benchmark" == "xyes"])

dnl The GTK+ tools are on by default
AC_ARG_WITH([gtk],
    AS_HELP_STRING([--without-gtk], [Disable GTK+ based tools]))
AM_CONDITIONAL(GTK, [test "x$with_gtk" != "xno"])

dnl The floating point build is on by default.
AC_ARG_WITH([fpu],
    AS_HELP_STRING([--without-fpu], [Disable FPU support (operate critical paths in fixed point)]))
dnl change to = "xyes" to disable by default FPU by default
AS_IF([test "x$with_fpu" != "xno"], [
	AC_DEFINE([HAVE_FPU])
])

dnl Range checking is off by default.
AC_ARG_ENABLE([range-check],
    AS_HELP_STRING([--enable-range-check], [Enable range checking (check for potential fixed point overflow)]))
AS_IF([test "x$enable_range_check" == "xyes"], [
	AC_DEFINE([ENABLE_RANGE_CHECK])
])

dnl ALSA is optional (although the tools prefer pulseaudio if they can get it)
PKG_CHECK_MODULES([ALSA], [alsa >= 1.0],
	AM_CONDITIONAL(HAVE_ALSA, true)
          AC_DEFINE(HAVE_ALSA,1,[Have ALSA]),
	AM_CONDITIONAL(HAVE_ALSA, false))

dnl Jack is mandatory (for now)
PKG_CHECK_MODULES([JACK], [jack >= 0.9])

dnl Pulseaudio is optional (but tintdrum won't build without it)
PKG_CHECK_MODULES([PULSE], [libpulse-simple >= 1.1],
	AM_CONDITIONAL(HAVE_PULSE, true)
          AC_DEFINE(HAVE_PULSE,1,[Have pulseaudio]),
	AM_CONDITIONAL(HAVE_PULSE, false))

AS_IF([test "x$with_gtk" != "xno"], [
   PKG_CHECK_MODULES([GTK], [gtk+-3.0 glib-2.0])
])

dnl This flag is required for the GTK+ signal autoconnection to work
AX_APPEND_LINK_FLAGS([-export-dynamic])

dnl Keep these near the bottom - adding -Werror breaks various tests
AX_CFLAGS_WARN_ALL
AX_APPEND_COMPILE_FLAGS([-Werror])

AC_CONFIG_FILES(Makefile src/Makefile src/librfn/Makefile src/libtt/Makefile src/jacklab/Makefile src/jacklua/Makefile src/tintamp/Makefile src/tintdrum/Makefile tests/Makefile)
AC_OUTPUT

