
bin_PROGRAMS = src/bin/etui

src_bin_etui_SOURCES = \
src/bin/etui_doc.c \
src/bin/etui_main.c \
src/bin/etui_theme.c \
src/bin/etui_win.c \
src/bin/etui_doc.h \
src/bin/etui_private.h \
src/bin/etui_theme.h \
src/bin/etui_win.h

src_bin_etui_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\" \
@ETUI_BIN_CFLAGS@

src_bin_etui_LDADD = \
$(top_builddir)/src/lib/libetui.la \
@ETUI_BIN_LIBS@

if HAVE_WIN32
src_bin_etui_SOURCES += \
src/bin/etui_res.rc

windres_verbose = $(windres_verbose_@AM_V@)
windres_verbose_ = $(windres_verbose_@AM_DEFAULT_V@)
windres_verbose_0 = $(AM_V_at)echo "  RC      " $@;

.rc.o:
	$(wc_verbose)windres -o $@ $<
endif
