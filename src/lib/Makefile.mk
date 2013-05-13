
lib_LTLIBRARIES = src/lib/libetui.la

includes_HEADERS = src/lib/Etui.h
includesdir = $(pkgincludedir)-@VMAJ@

src_lib_libetui_la_SOURCES = \
src/lib/etui_main.c \
src/lib/etui_module.c \
src/lib/etui_smart.c \
src/lib/etui_private.h

src_lib_libetui_la_CPPFLAGS = \
-DPACKAGE_BIN_DIR=\"$(bindir)\" \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\" \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@

src_lib_libetui_la_LIBADD = @ETUI_LIBS@

src_lib_libetui_la_LDFLAGS = -no-undefined -version-info @version_info@ @release_info@