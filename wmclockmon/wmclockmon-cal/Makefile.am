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
LIBS += $(gtk_LIBS)

INCLUDES = @HEADER_SEARCH_PATH@

wmclockmon_LDADD = @LIBRARY_SEARCH_PATH@ $(x11_LIBS) $(xext_LIBS) $(xpm_LIBS)

INSTALL_PROGRAM = \
	@INSTALL_PROGRAM@ \
	@SETGID_FLAGS@ \
	$(AM_INSTALL_PROGRAM_FLAGS)
