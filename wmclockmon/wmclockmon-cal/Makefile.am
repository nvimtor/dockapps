bin_PROGRAMS = wmclockmon-cal

wmclockmon_cal_SOURCES =   \
	defines.h \
	main.c \
	main.h \
	mainwindow.c \
	mainwindow.h \
	tools.c \
	tools.h

CPPFLAGS = @CPPFLAGS@ @DFLAGS@

AM_CFLAGS = $(gtk_CFLAGS)
LIBS += $(gtk_LIBS)

INCLUDES = @HEADER_SEARCH_PATH@

wmclockmon_LDADD = @LIBRARY_SEARCH_PATH@ @XLIBS@

INSTALL_PROGRAM = \
	@INSTALL_PROGRAM@ \
	@SETGID_FLAGS@ \
	$(AM_INSTALL_PROGRAM_FLAGS)
