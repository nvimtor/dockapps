dnl Process this file with autoconf to produce a configure script.
AC_PREREQ([2.52])

AC_INIT([wmclockmon], [0.8.1], [wmaker-dev@googlegroups.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([no-dist-gzip dist-xz])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])

AC_CANONICAL_HOST

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

AX_C___ATTRIBUTE__
AM_CONDITIONAL([HAVE___ATTRIBUTE__], [test "$ax_cv___attribute__" = "yes"])

PKG_CHECK_MODULES([gtk], [gtk+-3.0])

dnl ===========================================
dnl             Stuff that uses X
dnl ===========================================

PKG_CHECK_MODULES([x11],  [x11])
PKG_CHECK_MODULES([xext], [xext])
PKG_CHECK_MODULES([xpm],  [xpm])

dnl ===============================================
dnl             End of stuff that uses X
dnl ===============================================

dnl =========
dnl Debugging
dnl =========

AC_ARG_ENABLE([debug],
              [AS_HELP_STRING([--enable-debug], [turn on debugging [default=no]])])
AS_IF([test "$enable_debug" = yes], [
  debug_CPPFLAGS="-D_XOPEN_SOURCE=700 -DGTK_DISABLE_SINGLE_INCLUDES -DGTK_DISABLE_DEPRECATED"
  debug_CFLAGS="-Wall -Wextra -std=c11 -pedantic"
  AC_DEFINE([DEBUG], [1], [use debug code])
])
AC_SUBST([debug_CPPFLAGS])
AC_SUBST([debug_CFLAGS])


dnl ============
dnl Check for OS
dnl ============

AS_CASE([${host_os}],
        [freebsd*], [
          LIBS="$LIBS -lkvm"
          SETGID_FLAGS="-g kmem -m 2755 -o root"
        ],
        [linux*|netbsd*|openbsd*|solaris*], [],
        [AC_MSG_ERROR([Sorry, ${host_os} is not supported yet])])
AC_SUBST([SETGID_FLAGS])

dnl Checks for header files.
AC_CHECK_HEADERS([fcntl.h memory.h stddef.h stdlib.h string.h strings.h sys/param.h sys/time.h unistd.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([select strtoul uname])

AC_CONFIG_FILES([Makefile
		 doc/Makefile
		 src/Makefile
		 styles/Makefile
		 wmclockmon-config/Makefile
		 wmclockmon-cal/Makefile])
AC_OUTPUT
