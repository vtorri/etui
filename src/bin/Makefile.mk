
bin_PROGRAMS = src/bin/etui_test
#src/bin/etui

src_bin_etui_test_SOURCES = \
src/bin/etui.c

if HAVE_WIN32
src_bin_etui_test_SOURCES += \
src/bin/etui_res.rc
endif

src_bin_etui_test_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\" \
@ETUI_BIN_CFLAGS@

src_bin_etui_test_LDADD = \
$(top_builddir)/src/lib/libetui.la \
@ETUI_BIN_LIBS@

src_bin_etui_SOURCES = \
src/bin/config.c \
src/bin/main.c \
src/bin/etui_config.h \
src/bin/private.h

if HAVE_WIN32
src_bin_etui_SOURCES += \
src/bin/etui_res.rc
endif

src_bin_etui_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(bindir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\" \
@ETUI_BIN_CFLAGS@

src_bin_etui_LDADD = \
$(top_builddir)/src/lib/libetui.la \
@ETUI_BIN_LIBS@

if HAVE_WIN32
windres_verbose = $(windres_verbose_@AM_V@)
windres_verbose_ = $(windres_verbose_@AM_DEFAULT_V@)
windres_verbose_0 = $(AM_V_at)echo "  RES     " $@;

.rc.o:
	$(windres_verbose)windres -o $@ $<
endif
