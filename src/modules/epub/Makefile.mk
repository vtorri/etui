
if ETUI_BUILD_STATIC_EPUB

src_lib_libetui_la_SOURCES += \
src/modules/epub/etui_module_epub.c \
src/modules/epub/etui_module_epub.h

src_lib_libetui_la_CPPFLAGS += \
-I$(top_srcdir)/src/lib \
@EPUB_CFLAGS@

src_lib_libetui_la_CFLAGS += \
@ETUI_LIB_CFLAGS@

src_lib_libetui_la_LIBADD += \
@EPUB_LIBS@

else

etui_modules_epubdir = $(pkglibdir)/modules/epub/$(MODULE_ARCH)
etui_modules_epub_LTLIBRARIES = src/modules/epub/module.la

src_modules_epub_module_la_SOURCES = \
src/modules/epub/etui_module_epub.c \
src/modules/epub/etui_module_epub.h

src_modules_epub_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@EPUB_CFLAGS@

src_modules_epub_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_epub_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@EPUB_LIBS@

src_modules_epub_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_epub_module_la_LIBTOOLFLAGS = --tag=disable-static

endif
