gooroomappletdir = $(pkglibdir)
gooroomapplet_LTLIBRARIES = libgooroom-indicator-applet.la

libgooroom_indicator_applet_la_CPPFLAGS =	\
	-I$(srcdir) \
	-DINDICATOR_DIR=\""$(INDICATORDIR)"\" \
	-DINDICATOR_ICONS_DIR=\""$(INDICATORICONSDIR)"\" \
	-DGNOMELOCALEDIR=\""$(localedir)"\" \
	-DGDK_DISABLE_DEPRECATED

libgooroom_indicator_applet_la_SOURCES = \
	indicator-button.h	\
	indicator-button.c	\
	gooroom-indicator-applet.h	\
	gooroom-indicator-applet.c

libgooroom_indicator_applet_la_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GTK_CFLAGS) \
	$(GNOME_APPLETS_CFLAGS) \
	$(AYATANA_INDICATOR_NG_CFLAGS) \
	$(AM_CFLAGS)

libgooroom_indicator_applet_la_LDFLAGS = \
	-avoid-version \
	$(AM_LDFLAGS)

libgooroom_indicator_applet_la_LIBADD = \
	$(GLIB_LIBS) \
	$(GTK_LIBS) \
	$(GNOME_APPLETS_LIBS)	\
	$(AYATANA_INDICATOR_NG_LIBS)
