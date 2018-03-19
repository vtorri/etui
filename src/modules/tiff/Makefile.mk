
if ETUI_BUILD_STATIC_TIFF

src_lib_libetui_la_SOURCES += \
src/modules/tiff/etui_module_tiff.c \
src/modules/tiff/etui_module_tiff.h

src_lib_libetui_la_CPPFLAGS += \
-I$(top_srcdir)/src/lib \
@TIFF_CFLAGS@

src_lib_libetui_la_CFLAGS += \
@ETUI_LIB_CFLAGS@

src_lib_libetui_la_LIBADD += \
@TIFF_LIBS@

else

etui_modules_tiffdir = $(pkglibdir)/modules/tiff/$(MODULE_ARCH)
etui_modules_tiff_LTLIBRARIES = src/modules/tiff/module.la

src_modules_tiff_module_la_SOURCES = \
src/modules/tiff/etui_module_tiff.c \
src/modules/tiff/etui_module_tiff.h

src_modules_tiff_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@TIFF_CFLAGS@

src_modules_tiff_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_tiff_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@TIFF_LIBS@

src_modules_tiff_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_tiff_module_la_LIBTOOLFLAGS = --tag=disable-static

endif
