
noinst_PROGRAMS = \
src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) \
src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT)

BUILT_SOURCES = \
src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) \
src/modules/pdf/mupdf-1.2/generated/cmap_cns.h \
src/modules/pdf/mupdf-1.2/generated/cmap_gb.h \
src/modules/pdf/mupdf-1.2/generated/cmap_japan.h \
src/modules/pdf/mupdf-1.2/generated/cmap_korea.h \
src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) \
src/modules/pdf/mupdf-1.2/generated/font_base14.h \
src/modules/pdf/mupdf-1.2/generated/font_droid.h \
src/modules/pdf/mupdf-1.2/generated/font_cjk.h \
src/modules/pdf/mupdf-1.2/generated/font_cjk_full.h

src_modules_pdf_mupdf_1_2_scripts_cmapdump_SOURCES = src/modules/pdf/mupdf-1.2/scripts/cmapdump.c

src_modules_pdf_mupdf_1_2_scripts_cmapdump_CPPFLAGS = \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.2/fitz \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.2/pdf

src_modules_pdf_mupdf_1_2_scripts_fontdump_SOURCES = src/modules/pdf/mupdf-1.2/scripts/fontdump.c

src_modules_pdf_mupdf_1_2_scripts_fontdump_CPPFLAGS =

cmap_cns_files = \
src/modules/pdf/mupdf-1.2/cmaps/cns/Adobe-CNS1-0 \
src/modules/pdf/mupdf-1.2/cmaps/cns/Adobe-CNS1-1 \
src/modules/pdf/mupdf-1.2/cmaps/cns/Adobe-CNS1-2 \
src/modules/pdf/mupdf-1.2/cmaps/cns/Adobe-CNS1-3 \
src/modules/pdf/mupdf-1.2/cmaps/cns/Adobe-CNS1-4 \
src/modules/pdf/mupdf-1.2/cmaps/cns/Adobe-CNS1-5 \
src/modules/pdf/mupdf-1.2/cmaps/cns/Adobe-CNS1-6 \
src/modules/pdf/mupdf-1.2/cmaps/cns/Adobe-CNS1-UCS2 \
src/modules/pdf/mupdf-1.2/cmaps/cns/B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/B5pc-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/B5pc-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/CNS1-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/CNS1-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/CNS2-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/CNS2-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/CNS-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/CNS-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/ETen-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/ETen-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/ETenms-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/ETenms-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/ETHK-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/ETHK-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKdla-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKdla-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKdlb-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKdlb-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKgccs-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKgccs-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKm314-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKm314-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKm471-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKm471-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKscs-B5-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/HKscs-B5-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/UniCNS-UCS2-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/UniCNS-UCS2-V \
src/modules/pdf/mupdf-1.2/cmaps/cns/UniCNS-UTF16-H \
src/modules/pdf/mupdf-1.2/cmaps/cns/UniCNS-UTF16-V

cmap_gb_files = \
src/modules/pdf/mupdf-1.2/cmaps/gb/Adobe-GB1-0 \
src/modules/pdf/mupdf-1.2/cmaps/gb/Adobe-GB1-1 \
src/modules/pdf/mupdf-1.2/cmaps/gb/Adobe-GB1-2 \
src/modules/pdf/mupdf-1.2/cmaps/gb/Adobe-GB1-3 \
src/modules/pdf/mupdf-1.2/cmaps/gb/Adobe-GB1-4 \
src/modules/pdf/mupdf-1.2/cmaps/gb/Adobe-GB1-5 \
src/modules/pdf/mupdf-1.2/cmaps/gb/Adobe-GB1-UCS2 \
src/modules/pdf/mupdf-1.2/cmaps/gb/GB-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GB-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/GB-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBK2K-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBK2K-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBK-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBK-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBKp-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBKp-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBpc-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBpc-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBT-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBT-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBT-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBTpc-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBTpc-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/GBT-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/GB-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/UniGB-UCS2-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/UniGB-UCS2-V \
src/modules/pdf/mupdf-1.2/cmaps/gb/UniGB-UTF16-H \
src/modules/pdf/mupdf-1.2/cmaps/gb/UniGB-UTF16-V

cmap_japan_files = \
src/modules/pdf/mupdf-1.2/cmaps/japan/78-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/78-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/78-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/78ms-RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/78ms-RKSJ-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/78-RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/78-RKSJ-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/78-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/83pv-RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/90msp-RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/90msp-RKSJ-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/90ms-RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/90ms-RKSJ-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/90pv-RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/90pv-RKSJ-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/Add-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/Add-RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/Add-RKSJ-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/Add-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan1-0 \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan1-1 \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan1-2 \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan1-3 \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan1-4 \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan1-5 \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan1-6 \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan1-UCS2 \
src/modules/pdf/mupdf-1.2/cmaps/japan/Adobe-Japan2-0 \
src/modules/pdf/mupdf-1.2/cmaps/japan/EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/Ext-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/Ext-RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/Ext-RKSJ-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/Ext-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/H \
src/modules/pdf/mupdf-1.2/cmaps/japan/Hankaku \
src/modules/pdf/mupdf-1.2/cmaps/japan/Hiragana \
src/modules/pdf/mupdf-1.2/cmaps/japan/Hojo-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/Hojo-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/Hojo-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/Hojo-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/Katakana \
src/modules/pdf/mupdf-1.2/cmaps/japan/NWP-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/NWP-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/RKSJ-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/RKSJ-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/Roman \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniHojo-UCS2-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniHojo-UCS2-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniHojo-UTF16-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniHojo-UTF16-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniJISPro-UCS2-HW-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniJISPro-UCS2-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniJIS-UCS2-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniJIS-UCS2-HW-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniJIS-UCS2-HW-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniJIS-UCS2-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniJIS-UTF16-H \
src/modules/pdf/mupdf-1.2/cmaps/japan/UniJIS-UTF16-V \
src/modules/pdf/mupdf-1.2/cmaps/japan/V \
src/modules/pdf/mupdf-1.2/cmaps/japan/WP-Symbol

cmap_korea_files = \
src/modules/pdf/mupdf-1.2/cmaps/korea/Adobe-Korea1-0 \
src/modules/pdf/mupdf-1.2/cmaps/korea/Adobe-Korea1-1 \
src/modules/pdf/mupdf-1.2/cmaps/korea/Adobe-Korea1-2 \
src/modules/pdf/mupdf-1.2/cmaps/korea/Adobe-Korea1-UCS2 \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSC-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSC-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSC-H \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSC-Johab-H \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSC-Johab-V \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSCms-UHC-H \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSCms-UHC-HW-H \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSCms-UHC-HW-V \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSCms-UHC-V \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSCpc-EUC-H \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSCpc-EUC-V \
src/modules/pdf/mupdf-1.2/cmaps/korea/KSC-V \
src/modules/pdf/mupdf-1.2/cmaps/korea/UniKS-UCS2-H \
src/modules/pdf/mupdf-1.2/cmaps/korea/UniKS-UCS2-V \
src/modules/pdf/mupdf-1.2/cmaps/korea/UniKS-UTF16-H \
src/modules/pdf/mupdf-1.2/cmaps/korea/UniKS-UTF16-V

font_base14_files = \
src/modules/pdf/mupdf-1.2/fonts/Dingbats.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusMonL-Bold.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusMonL-BoldObli.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusMonL-Regu.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusMonL-ReguObli.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusRomNo9L-Medi.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusRomNo9L-MediItal.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusRomNo9L-Regu.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusRomNo9L-ReguItal.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusSanL-Bold.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusSanL-BoldItal.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusSanL-Regu.cff \
src/modules/pdf/mupdf-1.2/fonts/NimbusSanL-ReguItal.cff \
src/modules/pdf/mupdf-1.2/fonts/StandardSymL.cff

font_droid_files = \
src/modules/pdf/mupdf-1.2/fonts/droid/DroidSans.ttf \
src/modules/pdf/mupdf-1.2/fonts/droid/DroidSansMono.ttf

font_cjk_files = \
src/modules/pdf/mupdf-1.2/fonts/droid/DroidSansFallback.ttf

font_cjk_full_files = \
src/modules/pdf/mupdf-1.2/fonts/droid/DroidSansFallbackFull.ttf

src/modules/pdf/mupdf-1.2/generated:
	@echo "  MKDIR  generated/"
	@$(MKDIR_P) src/modules/pdf/mupdf-1.2/generated/

src/modules/pdf/mupdf-1.2/generated/cmap_cns.h: src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) src/modules/pdf/mupdf-1.2/generated $(cmap_cns_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) $@ $(cmap_cns_files)

src/modules/pdf/mupdf-1.2/generated/cmap_gb.h: src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) src/modules/pdf/mupdf-1.2/generated $(cmap_gb_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) $@ $(cmap_gb_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.2/generated/cmap_japan.h: src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) src/modules/pdf/mupdf-1.2/generated $(cmap_japan_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) $@ $(cmap_japan_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.2/generated/cmap_korea.h: src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) src/modules/pdf/mupdf-1.2/generated $(cmap_korea_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.2/scripts/cmapdump$(EXEEXT) $@ $(cmap_korea_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.2/generated/font_base14.h: src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) src/modules/pdf/mupdf-1.2/generated $(font_base14_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) $@ $(font_base14_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.2/generated/font_droid.h: src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) src/modules/pdf/mupdf-1.2/generated $(font_droid_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) $@ $(font_droid_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.2/generated/font_cjk.h: src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) src/modules/pdf/mupdf-1.2/generated $(font_cjk_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) $@ $(font_cjk_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.2/generated/font_cjk_full.h: src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) src/modules/pdf/mupdf-1.2/generated $(font_cjk_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.2/scripts/fontdump$(EXEEXT) $@ $(font_cjk_full_files) > /dev/null 2>&1


FITZ_SOURCES = \
src/modules/pdf/mupdf-1.2/fitz/base_context.c \
src/modules/pdf/mupdf-1.2/fitz/base_error.c \
src/modules/pdf/mupdf-1.2/fitz/base_geometry.c \
src/modules/pdf/mupdf-1.2/fitz/base_getopt.c \
src/modules/pdf/mupdf-1.2/fitz/base_hash.c \
src/modules/pdf/mupdf-1.2/fitz/base_memory.c \
src/modules/pdf/mupdf-1.2/fitz/base_string.c \
src/modules/pdf/mupdf-1.2/fitz/base_time.c \
src/modules/pdf/mupdf-1.2/fitz/base_trans.c \
src/modules/pdf/mupdf-1.2/fitz/base_xml.c \
src/modules/pdf/mupdf-1.2/fitz/crypt_aes.c \
src/modules/pdf/mupdf-1.2/fitz/crypt_arc4.c \
src/modules/pdf/mupdf-1.2/fitz/crypt_md5.c \
src/modules/pdf/mupdf-1.2/fitz/crypt_sha2.c \
src/modules/pdf/mupdf-1.2/fitz/dev_bbox.c \
src/modules/pdf/mupdf-1.2/fitz/dev_list.c \
src/modules/pdf/mupdf-1.2/fitz/dev_null.c \
src/modules/pdf/mupdf-1.2/fitz/dev_text.c \
src/modules/pdf/mupdf-1.2/fitz/dev_trace.c \
src/modules/pdf/mupdf-1.2/fitz/doc_document.c \
src/modules/pdf/mupdf-1.2/fitz/doc_interactive.c \
src/modules/pdf/mupdf-1.2/fitz/doc_link.c \
src/modules/pdf/mupdf-1.2/fitz/doc_outline.c \
src/modules/pdf/mupdf-1.2/fitz/doc_search.c \
src/modules/pdf/mupdf-1.2/fitz/filt_basic.c \
src/modules/pdf/mupdf-1.2/fitz/filt_dctd.c \
src/modules/pdf/mupdf-1.2/fitz/filt_faxd.c \
src/modules/pdf/mupdf-1.2/fitz/filt_flate.c \
src/modules/pdf/mupdf-1.2/fitz/filt_jbig2d.c \
src/modules/pdf/mupdf-1.2/fitz/filt_lzwd.c \
src/modules/pdf/mupdf-1.2/fitz/filt_predict.c \
src/modules/pdf/mupdf-1.2/fitz/image_jpeg.c \
src/modules/pdf/mupdf-1.2/fitz/image_jpx.c \
src/modules/pdf/mupdf-1.2/fitz/image_md5.c \
src/modules/pdf/mupdf-1.2/fitz/image_png.c \
src/modules/pdf/mupdf-1.2/fitz/image_save.c \
src/modules/pdf/mupdf-1.2/fitz/image_tiff.c \
src/modules/pdf/mupdf-1.2/fitz/memento.c \
src/modules/pdf/mupdf-1.2/fitz/res_bitmap.c \
src/modules/pdf/mupdf-1.2/fitz/res_colorspace.c \
src/modules/pdf/mupdf-1.2/fitz/res_font.c \
src/modules/pdf/mupdf-1.2/fitz/res_halftone.c \
src/modules/pdf/mupdf-1.2/fitz/res_path.c \
src/modules/pdf/mupdf-1.2/fitz/res_pixmap.c \
src/modules/pdf/mupdf-1.2/fitz/res_shade.c \
src/modules/pdf/mupdf-1.2/fitz/res_store.c \
src/modules/pdf/mupdf-1.2/fitz/res_text.c \
src/modules/pdf/mupdf-1.2/fitz/stm_buffer.c \
src/modules/pdf/mupdf-1.2/fitz/stm_comp_buf.c \
src/modules/pdf/mupdf-1.2/fitz/stm_open.c \
src/modules/pdf/mupdf-1.2/fitz/stm_output.c \
src/modules/pdf/mupdf-1.2/fitz/stm_read.c \
src/modules/pdf/mupdf-1.2/fitz/fitz.h \
src/modules/pdf/mupdf-1.2/fitz/fitz-internal.h \
src/modules/pdf/mupdf-1.2/fitz/memento.h

DRAW_SOURCES = \
src/modules/pdf/mupdf-1.2/draw/draw_affine.c \
src/modules/pdf/mupdf-1.2/draw/draw_blend.c \
src/modules/pdf/mupdf-1.2/draw/draw_device.c \
src/modules/pdf/mupdf-1.2/draw/draw_edge.c \
src/modules/pdf/mupdf-1.2/draw/draw_glyph.c \
src/modules/pdf/mupdf-1.2/draw/draw_mesh.c \
src/modules/pdf/mupdf-1.2/draw/draw_paint.c \
src/modules/pdf/mupdf-1.2/draw/draw_path.c \
src/modules/pdf/mupdf-1.2/draw/draw_scale.c \
src/modules/pdf/mupdf-1.2/draw/draw_unpack.c

PDF_SOURCES = \
src/modules/pdf/mupdf-1.2/pdf/pdf_annot.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_cmap.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_cmap_load.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_cmap_parse.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_cmap_table.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_colorspace.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_crypt.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_device.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_encoding.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_event.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_font.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_fontfile.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_form.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_function.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_image.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_interpret.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_js_none.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_lex.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_metrics.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_nametree.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_object.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_outline.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_page.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_parse.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_pattern.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_repair.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_shade.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_store.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_stream.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_type3.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_unicode.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_write.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_xobject.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_xref_aux.c \
src/modules/pdf/mupdf-1.2/pdf/pdf_xref.c \
src/modules/pdf/mupdf-1.2/pdf/data_encodings.h \
src/modules/pdf/mupdf-1.2/pdf/data_glyphlist.h \
src/modules/pdf/mupdf-1.2/pdf/mupdf.h \
src/modules/pdf/mupdf-1.2/pdf/mupdf-internal.h

JBIG2DEC_SOURCES = \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_arith.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_arith_iaid.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_arith_int.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_generic.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_halftone.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_huffman.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_image.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_metadata.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_mmr.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_page.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_refinement.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_segment.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_symbol_dict.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_text.c \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_arith.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_arith_iaid.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_arith_int.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_generic.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_huffman.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_hufftab.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_image.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_metadata.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_mmr.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_priv.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_symbol_dict.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/jbig2_text.h \
src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec/os_types.h

XPS_SOURCES = \
src/modules/pdf/mupdf-1.2/xps/xps_common.c \
src/modules/pdf/mupdf-1.2/xps/xps_doc.c \
src/modules/pdf/mupdf-1.2/xps/xps_glyphs.c \
src/modules/pdf/mupdf-1.2/xps/xps_gradient.c \
src/modules/pdf/mupdf-1.2/xps/xps_image.c \
src/modules/pdf/mupdf-1.2/xps/xps_outline.c \
src/modules/pdf/mupdf-1.2/xps/xps_path.c \
src/modules/pdf/mupdf-1.2/xps/xps_resource.c \
src/modules/pdf/mupdf-1.2/xps/xps_tile.c \
src/modules/pdf/mupdf-1.2/xps/xps_util.c \
src/modules/pdf/mupdf-1.2/xps/xps_zip.c \
src/modules/pdf/mupdf-1.2/xps/muxps-internal.h \
src/modules/pdf/mupdf-1.2/xps/muxps.h

CBZ_SOURCES = \
src/modules/pdf/mupdf-1.2/cbz/mucbz.c \
src/modules/pdf/mupdf-1.2/cbz/mucbz.h


if ETUI_BUILD_STATIC_PDF

src_lib_libetui_la_SOURCES += \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h \
$(FITZ_SOURCES) \
$(DRAW_SOURCES) \
$(PDF_SOURCES) \
$(JBIG2DEC_SOURCES) \
$(XPS_SOURCES) \
$(CBZ_SOURCES)

src_lib_libetui_la_CPPFLAGS += \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.2/fitz \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.2/pdf \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec \
@MUPDF_CFLAGS@

src_lib_libetui_la_LIBADD += \
@MUPDF_LIBS@

else

etui_modules_pdfdir = $(pkglibdir)/modules/pdf/$(MODULE_ARCH)
etui_modules_pdf_LTLIBRARIES = src/modules/pdf/module.la

src_modules_pdf_module_la_SOURCES = \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h \
$(FITZ_SOURCES) \
$(DRAW_SOURCES) \
$(PDF_SOURCES) \
$(JBIG2DEC_SOURCES) \
$(XPS_SOURCES) \
$(CBZ_SOURCES)

src_modules_pdf_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.2/fitz \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.2/pdf \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.2/thirdparty/jbig2dec \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@MUPDF_CFLAGS@

src_modules_pdf_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@MUPDF_LIBS@

src_modules_pdf_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_pdf_module_la_LIBTOOLFLAGS = --tag=disable-static

endif

ETUI_CLEANFILES += src/modules/pdf/mupdf-1.2/generated

EXTRA_DIST += \
$(cmap_cns_files) \
$(cmap_gb_files) \
$(cmap_japan_files) \
$(cmap_korea_files) \
$(font_base_files) \
$(font_droid_files) \
$(font_cjk_files)
