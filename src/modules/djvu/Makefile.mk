
if ETUI_BUILD_STATIC_DJVU

src_lib_libetui_la_SOURCES += \
src/modules/djvu/etui_module_djvu.c \
src/modules/djvu/etui_module_djvu.h

src_lib_libetui_la_CPPFLAGS += \
-I$(top_srcdir)/src/lib \
@DJVU_CFLAGS@

src_lib_libetui_la_CFLAGS += \
@ETUI_LIB_CFLAGS@

src_lib_libetui_la_LIBADD += \
@DJVU_LIBS@

else

etui_modules_djvudir = $(pkglibdir)/modules/djvu/$(MODULE_ARCH)
etui_modules_djvu_LTLIBRARIES = src/modules/djvu/module.la

src_modules_djvu_module_la_SOURCES = \
src/modules/djvu/etui_module_djvu.c \
src/modules/djvu/etui_module_djvu.h

src_modules_djvu_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@DJVU_CFLAGS@

src_modules_djvu_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_djvu_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@DJVU_LIBS@

src_modules_djvu_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_djvu_module_la_LIBTOOLFLAGS = --tag=disable-static

endif
