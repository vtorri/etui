
if ETUI_BUILD_STATIC_IMG

src_lib_libetui_la_SOURCES += \
src/modules/img/etui_module_img.c \
src/modules/img/etui_module_img.h

src_lib_libetui_la_CPPFLAGS += \
@IMG_CFLAGS@

src_lib_libetui_la_CFLAGS += \
@ETUI_LIB_CFLAGS@

src_lib_libetui_la_CXXFLAGS = \
@ETUI_LIB_CXXFLAGS@

src_lib_libetui_la_LIBADD += \
@IMG_LIBS@

else

etui_modules_imgdir = $(pkglibdir)/modules/img/$(MODULE_ARCH)
etui_modules_img_LTLIBRARIES = src/modules/img/module.la

src_modules_img_module_la_SOURCES = \
src/modules/img/etui_module_img.c \
src/modules/img/etui_module_img.h

src_modules_img_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@IMG_CFLAGS@

src_modules_img_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_img_module_la_CXXFLAGS = \
@ETUI_LIB_CXXFLAGS@

src_modules_img_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@IMG_LIBS@

src_modules_img_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_img_module_la_LIBTOOLFLAGS = --tag=disable-static

endif
