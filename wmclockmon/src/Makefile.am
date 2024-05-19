include $(top_srcdir)/Makefile.common.am

bin_PROGRAMS = wmclockmon

wmclockmon_SOURCES =   \
	main.c \
	dockapp.c \
	dockapp.h \
	backlight0_off.xpm \
	backlight0_on.xpm \
	backlightI_off.xpm \
	backlightI_on.xpm \
	backlightB_off.xpm \
	backlightB_on.xpm \
	parts.xpm \
	letters.xpm

AM_CPPFLAGS = -DDATADIR="\"$(pkgdatadir)\""

AM_CFLAGS += $(x11_CFLAGS) $(xext_CFLAGS) $(xpm_CFLAGS)

LDADD = $(x11_LIBS) $(xext_LIBS) $(xpm_LIBS)
