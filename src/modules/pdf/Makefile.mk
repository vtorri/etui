
# MuPDF sources

## fitz

FITZ_SOURCES := \
src/modules/pdf/mupdf-1.10a/include/mupdf/fitz.h \
$(wildcard src/modules/pdf/mupdf-1.10a/include/mupdf/fitz/*.h) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/fitz/*.c) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/fitz/*.h)

## pdf

PDF_SOURCES := \
src/modules/pdf/mupdf-1.10a/include/mupdf/pdf.h \
$(wildcard src/modules/pdf/mupdf-1.10a/include/mupdf/pdf/*.h) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/pdf/*.c) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/pdf/*.h)

## html

HTML_SOURCES := \
src/modules/pdf/mupdf-1.10a/include/mupdf/html.h \
$(wildcard src/modules/pdf/mupdf-1.10a/source/html/*.c) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/html/*.h)

## svg

SVG_SOURCES := \
src/modules/pdf/mupdf-1.10a/include/mupdf/svg.h \
$(wildcard src/modules/pdf/mupdf-1.10a/source/svg/*.c) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/svg/*.h)

## cbz

CBZ_SOURCES := \
$(wildcard src/modules/pdf/mupdf-1.10a/source/cbz/*.c) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/cbz/*.h)

## xps

XPS_SOURCES := \
$(wildcard src/modules/pdf/mupdf-1.10a/source/xps/*.c) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/xps/*.h)

## gprf

GPRF_SOURCES := \
$(wildcard src/modules/pdf/mupdf-1.10a/source/gprf/*.c) \
$(wildcard src/modules/pdf/mupdf-1.10a/source/gprf/*.h)

MUPDF_SOURCES = \
$(FITZ_SOURCES) \
$(PDF_SOURCES) \
$(HTML_SOURCES) \
$(SVG_SOURCES) \
$(CBZ_SOURCES) \
$(XPS_SOURCES) \
$(GPRF_SOURCES)

# thirdparty sources

## jbig2dec

THIRD_JBIG2DEC_SOURCES = \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_arith.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_arith_iaid.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_arith_int.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_generic.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_halftone.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_huffman.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_image.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_metadata.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_mmr.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_page.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_refinement.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_segment.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_symbol_dict.c \
src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec/jbig2_text.c


# openjpeg

THIRD_OPENJPEG_SOURCES = \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/bio.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/cidx_manager.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/cio.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/dwt.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/event.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/function_list.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/image.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/invert.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/j2k.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/jp2.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/mct.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/mqc.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/openjpeg.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/opj_malloc.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/phix_manager.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/pi.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/ppix_manager.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/raw.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/t1.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/t2.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/tcd.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/tgt.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/thix_manager.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/thread.c \
src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2/tpix_manager.c

# mujs

THIRD_MUJS_SOURCES = \
src/modules/pdf/mupdf-1.10a/thirdparty/mujs/one.c

THIRD_SOURCES = \
$(THIRD_JBIG2DEC_SOURCES) \
$(THIRD_OPENJPEG_SOURCES) \
$(THIRD_MUJS_SOURCES)

if ETUI_BUILD_STATIC_PDF

src_lib_libetui_la_SOURCES += \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h \
$(MUPDF_SOURCES)

src_lib_libetui_la_CPPFLAGS += \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/include

src_lib_libetui_la_LIBADD += \
@MUPDF_LIBS@

else

etui_modules_pdfdir = $(pkglibdir)/modules/pdf/$(MODULE_ARCH)
etui_modules_pdf_LTLIBRARIES = src/modules/pdf/module.la

src_modules_pdf_module_la_SOURCES = \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h

$(src_modules_pdf_module_la_OBJECTS): build-mupdf

build-mupdf:
	cd src/modules/pdf/mupdf-1.10a && make

src_modules_pdf_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/include \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@

src_modules_pdf_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_pdf_module_la_LIBADD = \
src/lib/libetui.la \
-L$(abs_srcdir)/src/modules/pdf/mupdf-1.10a/build/release -lmupdf \
-L$(abs_srcdir)/src/modules/pdf/mupdf-1.10a/build/release -lmupdfthird \
@ETUI_LIBS@

src_modules_pdf_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_pdf_module_la_LIBTOOLFLAGS = --tag=disable-static

endif

# EXTRA_DIST

## fonts

FONT_SOURCES_DROID := $(wildcard src/modules/pdf/mupdf-1.10a/resources/fonts/droid/*.ttf)
FONT_SOURCES_NOTO := $(wildcard src/modules/pdf/mupdf-1.10a/resources/fonts/noto/*.ttf)
FONT_SOURCES_HAN := $(wildcard src/modules/pdf/mupdf-1.10a/resources/fonts/han/*.otf)
FONT_SOURCES_URW := $(wildcard src/modules/pdf/mupdf-1.10a/resources/fonts/urw/*.cff)
FONT_SOURCES_SIL := $(wildcard src/modules/pdf/mupdf-1.10a/resources/fonts/sil/*.cff)

FONT_SOURCES := \
$(FONT_SOURCES_DROID) \
$(FONT_SOURCES_NOTO) \
$(FONT_SOURCES_HAN) \
$(FONT_SOURCES_URW) \
$(FONT_SOURCES_SIL)

## cmaps

CMAP_SOURCES_CNS := $(wildcard src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/*)
CMAP_SOURCES_GB := $(wildcard src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/*)
CMAP_SOURCES_JAPAN := $(wildcard src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/*)
CMAP_SOURCES_KOREA := $(wildcard src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/*)

CMAP_SOURCES := \
$(CMAP_SOURCES_CNS) \
$(CMAP_SOURCES_GB) \
$(CMAP_SOURCES_JAPAN) \
$(CMAP_SOURCES_KOREA)

EXTRA_DIST += \
src/modules/pdf/mupdf-1.10a/CHANGES \
src/modules/pdf/mupdf-1.10a/CONTRIBUTORS \
src/modules/pdf/mupdf-1.10a/COPYING \
src/modules/pdf/mupdf-1.10a/Makefile \
src/modules/pdf/mupdf-1.10a/Makerules \
src/modules/pdf/mupdf-1.10a/Makethird \
src/modules/pdf/mupdf-1.10a/README \
$(MUPDF_SOURCES) \
$(THIRD_SOURCES) \
$(CMAP_SOURCES) \
$(FONT_SOURCES)
