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
PKG_CHECK_MODULES([jack], [jack >= 0.9])

dnl Currently the floating point build is on by default.
AC_ARG_WITH([fpu],
    AS_HELP_STRING([--without-fpu], [Disable FPU support (operate critical paths in fixed point)]))
dnl change to = "xyes" to disable by default FPU by default
AS_IF([test "x$with_fpu" != "xno"], [
	AC_DEFINE([HAVE_FPU])
])

dnl Keep these near the bottom - adding -Werror breaks various tests
AX_CFLAGS_WARN_ALL
AX_APPEND_COMPILE_FLAGS([-Werror])

AC_CONFIG_FILES(Makefile src/Makefile src/librfn/Makefile src/libtt/Makefile src/jacklua/Makefile tests/Makefile)
AC_OUTPUT

