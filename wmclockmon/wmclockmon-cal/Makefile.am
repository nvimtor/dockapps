include $(top_srcdir)/Makefile.common.am

bin_PROGRAMS = wmclockmon-cal

wmclockmon_cal_SOURCES =   \
	defines.h \
	main.c \
	main.h \
	mainwindow.c \
	mainwindow.h \
	tools.c \
	tools.h

AM_CFLAGS += $(gtk_CFLAGS) $(x11_CFLAGS) $(xext_CFLAGS) $(xpm_CFLAGS)

LDADD = $(gtk_LIBS) $(x11_LIBS) $(xext_LIBS) $(xpm_LIBS)
