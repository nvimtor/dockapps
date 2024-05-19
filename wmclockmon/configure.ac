dnl Process this file with autoconf to produce a configure script.
AC_PREREQ(2.52)

dnl AC_INIT(src/main.c)
dnl AM_INIT_AUTOMAKE(wmclockmon, 0.8.1)
AC_INIT(wmclockmon, 0.8.1, tnemeth@free.fr)
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE($PACKAGE_NAME, $PACKAGE_VERSION)

AC_CONFIG_SRCDIR(src/main.c)
AM_CONFIG_HEADER(config.h)

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S

PKG_CHECK_MODULES([gtk], [gtk+-2.0])

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
if test "$enable_debug" = yes; then
  debug_CFLAGS="-Wall -g -ansi -pedantic"
  AC_DEFINE(DEBUG, 1, [use debug code])
fi
AC_SUBST([debug_CFLAGS])


dnl ============
dnl Check for OS
dnl ============

ignore_buffers=no
ignore_cached=no
ignore_wired=no

case ${host_os} in
linux*)
  OS=linux
  ignore_buffers=yes
  ignore_cached=yes
  ;;
freebsd*)
  OS=freebsd
  ignore_wired=yes
  ignore_cached=yes
  LIBS="$LIBS -lkvm"
  SETGID_FLAGS="-g kmem -m 2755 -o root"
  ;;
openbsd*)
  OS=openbsd
  ;;
netbsd*)
  OS=netbsd
  ;;
solaris*)
  OS=solaris
  ;;
*)
  echo ""
  echo "Sorry, ${host_os} is not supported yet"
  echo ""
  exit 1
  ;;
esac

AC_SUBST(OS)
AC_SUBST(SETGID_FLAGS)
AC_SUBST(LIBS)

if test "$ignore_buffers" = yes; then
  AC_DEFINE(IGNORE_BUFFERS, 1, [use '--ignore-buffers' option])
fi
if test "$ignore_cached" = yes; then
  AC_DEFINE(IGNORE_CACHED, 1, [use '--ignore-cached' option])
fi
if test "$ignore_wired" = yes; then
  AC_DEFINE(IGNORE_WIRED, 1, [use '--ignore-wired' option])
fi


dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h memory.h stddef.h stdlib.h string.h strings.h sys/param.h sys/time.h unistd.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS(select strtoul uname)

AC_CONFIG_FILES(Makefile \
		src/Makefile \
		doc/Makefile \
		wmclockmon-config/Makefile\
		wmclockmon-cal/Makefile\
		styles/Makefile)
AC_OUTPUT
