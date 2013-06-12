
if ETUI_BUILD_STATIC_PS

src_lib_libetui_la_SOURCES += \
src/modules/ps/ps.c \
src/modules/ps/etui_module_ps.c \
src/modules/ps/etui_module_ps_utils.c \
src/modules/ps/ps.h \
src/modules/ps/etui_module_ps.h \
src/modules/ps/etui_module_ps_utils.h

else

etui_modules_psdir = $(pkglibdir)/modules/ps/$(MODULE_ARCH)
etui_modules_ps_LTLIBRARIES = src/modules/ps/module.la

src_modules_ps_module_la_SOURCES = \
src/modules/ps/ps.c \
src/modules/ps/etui_module_ps.c \
src/modules/ps/etui_module_ps_utils.c \
src/modules/ps/ps.h \
src/modules/ps/etui_module_ps.h \
src/modules/ps/etui_module_ps_utils.h

src_modules_ps_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@PS_CFLAGS@

src_modules_ps_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@PS_LIBS@

src_modules_ps_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_ps_module_la_LIBTOOLFLAGS = --tag=disable-static

endif
