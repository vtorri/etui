
if ETUI_BUILD_STATIC_PDF

src_lib_libetui_la_SOURCES += \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h

src_lib_libetui_la_CPPFLAGS += \
@MUPDF_CFLAGS@

src_lib_libetui_la_LIBADD += \
@MUPDF_LIBS@

else

etui_modules_pdfdir = $(pkglibdir)/modules/pdf/$(MODULE_ARCH)
etui_modules_pdf_LTLIBRARIES = src/modules/pdf/module.la

src_modules_pdf_module_la_SOURCES = \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h

src_modules_pdf_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@MUPDF_CFLAGS@

src_modules_pdf_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_pdf_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@

src_modules_pdf_module_la_LDFLAGS = \
-no-undefined \
-module \
-avoid-version \
-Wl,@MUPDF_LIBS@/libmupdf.a \
-Wl,@MUPDF_LIBS@/libmupdfthird.a

src_modules_pdf_module_la_LIBTOOLFLAGS = --tag=disable-static

endif
