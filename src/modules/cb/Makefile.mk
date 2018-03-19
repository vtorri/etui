
if ETUI_BUILD_STATIC_CB

src_lib_libetui_la_SOURCES += \
src/modules/cb/etui_module_cb.c \
src/modules/cb/etui_module_cb.h

src_lib_libetui_la_CPPFLAGS += \
-I$(top_srcdir)/src/lib \
@CB_CFLAGS@

src_lib_libetui_la_CFLAGS += \
@ETUI_LIB_CFLAGS@

src_lib_libetui_la_LIBADD += \
@CB_LIBS@

else

etui_modules_cbdir = $(pkglibdir)/modules/cb/$(MODULE_ARCH)
etui_modules_cb_LTLIBRARIES = src/modules/cb/module.la

src_modules_cb_module_la_SOURCES = \
src/modules/cb/etui_module_cb.c \
src/modules/cb/etui_module_cb.h

src_modules_cb_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@CB_CFLAGS@

src_modules_cb_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_cb_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@CB_LIBS@

src_modules_cb_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_cb_module_la_LIBTOOLFLAGS = --tag=disable-static

endif
