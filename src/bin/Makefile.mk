
bin_PROGRAMS = src/bin/etui src/bin/etui_elm

src_bin_etui_SOURCES = src/bin/etui.c

src_bin_etui_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
@ETUI_BIN_CFLAGS@

src_bin_etui_LDADD = \
$(top_builddir)/src/lib/libetui.la \
@ETUI_BIN_LIBS@

src_bin_etui_elm_SOURCES = src/bin/etui_elm.c

src_bin_etui_elm_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
@ETUI_BIN_CFLAGS@

src_bin_etui_elm_LDADD = \
$(top_builddir)/src/lib/libetui.la \
@ETUI_BIN_LIBS@
