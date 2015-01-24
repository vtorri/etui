BASE_INCLUDES = \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/include

noinst_PROGRAMS = \
src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) \
src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) \
src/modules/pdf/mupdf-1.6/scripts/cquote$(EXEEXT)

BUILT_SOURCES = \
src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) \
src/modules/pdf/mupdf-1.6/generated/gen_cmap_cns.h \
src/modules/pdf/mupdf-1.6/generated/gen_cmap_gb.h \
src/modules/pdf/mupdf-1.6/generated/gen_cmap_japan.h \
src/modules/pdf/mupdf-1.6/generated/gen_cmap_korea.h \
src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) \
src/modules/pdf/mupdf-1.6/generated/gen_font_base14.h \
src/modules/pdf/mupdf-1.6/generated/gen_font_droid.h \
src/modules/pdf/mupdf-1.6/generated/gen_font_cjk.h \
src/modules/pdf/mupdf-1.6/generated/gen_font_cjk_full.h \
src/modules/pdf/mupdf-1.6/scripts/cquote$(EXEEXT) \
src/modules/pdf/mupdf-1.6/generated/gen_js_util.h

src_modules_pdf_mupdf_1_6_scripts_cmapdump_SOURCES = src/modules/pdf/mupdf-1.6/scripts/cmapdump.c

src_modules_pdf_mupdf_1_6_scripts_cmapdump_CPPFLAGS = \
$(BASE_INCLUDES)

src_modules_pdf_mupdf_1_6_scripts_fontdump_SOURCES = src/modules/pdf/mupdf-1.6/scripts/fontdump.c

src_modules_pdf_mupdf_1_6_scripts_fontdump_CPPFLAGS =

src_modules_pdf_mupdf_1_6_scripts_cquote_SOURCES = src/modules/pdf/mupdf-1.6/scripts/cquote.c

cmap_cns_files = \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/Adobe-CNS1-0 \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/Adobe-CNS1-1 \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/Adobe-CNS1-2 \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/Adobe-CNS1-3 \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/Adobe-CNS1-4 \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/Adobe-CNS1-5 \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/Adobe-CNS1-6 \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/Adobe-CNS1-UCS2 \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/B5pc-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/B5pc-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/CNS1-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/CNS1-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/CNS2-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/CNS2-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/CNS-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/CNS-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/ETen-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/ETen-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/ETenms-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/ETenms-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/ETHK-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/ETHK-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKdla-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKdla-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKdlb-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKdlb-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKgccs-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKgccs-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKm314-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKm314-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKm471-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKm471-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKscs-B5-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/HKscs-B5-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-UCS2-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-UCS2-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-UTF16-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-UTF16-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-UTF32-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-UTF32-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-UTF8-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-UTF8-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/cns/UniCNS-X

cmap_gb_files = \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/Adobe-GB1-0 \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/Adobe-GB1-1 \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/Adobe-GB1-2 \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/Adobe-GB1-3 \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/Adobe-GB1-4 \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/Adobe-GB1-5 \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/Adobe-GB1-UCS2 \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GB-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GB-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GB-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBK2K-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBK2K-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBK-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBK-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBKp-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBKp-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBpc-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBpc-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBT-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBT-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBT-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBTpc-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBTpc-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GBT-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/GB-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-UCS2-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-UCS2-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-UTF16-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-UTF16-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-UTF32-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-UTF32-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-UTF8-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-UTF8-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/gb/UniGB-X

cmap_japan_files = \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/78-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/78-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/78-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/78ms-RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/78ms-RKSJ-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/78-RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/78-RKSJ-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/78-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/83pv-RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/90msp-RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/90msp-RKSJ-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/90ms-RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/90ms-RKSJ-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/90pv-RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/90pv-RKSJ-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Add-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Add-RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Add-RKSJ-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Add-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Adobe-Japan1-0 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Adobe-Japan1-1 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Adobe-Japan1-2 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Adobe-Japan1-3 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Adobe-Japan1-4 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Adobe-Japan1-5 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Adobe-Japan1-6 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Adobe-Japan1-UCS2 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Ext-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Ext-RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Ext-RKSJ-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Ext-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Hankaku \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Hiragana \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Katakana \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/NWP-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/NWP-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/RKSJ-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/RKSJ-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/Roman \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS2004-UTF16-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS2004-UTF16-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS2004-UTF32-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS2004-UTF32-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS2004-UTF8-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS2004-UTF8-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJISPro-UCS2-HW-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJISPro-UCS2-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJISPro-UTF8-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UCS2-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UCS2-HW-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UCS2-HW-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UCS2-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UTF16-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UTF16-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UTF32-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UTF32-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UTF8-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-UTF8-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-X \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJISX02132004-UTF32-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJISX02132004-UTF32-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJISX0213-UTF32-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJISX0213-UTF32-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-X16 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-X32 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/UniJIS-X8 \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/V \
src/modules/pdf/mupdf-1.6/resources/cmaps/japan/WP-Symbol

cmap_korea_files = \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/Adobe-Korea1-0 \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/Adobe-Korea1-1 \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/Adobe-Korea1-2 \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/Adobe-Korea1-UCS2 \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSC-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSC-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSC-Johab-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSC-Johab-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSCms-UHC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSCms-UHC-HW-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSCms-UHC-HW-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSCms-UHC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSCpc-EUC-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSCpc-EUC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/KSC-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-UCS2-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-UCS2-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-UTF16-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-UTF16-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-UTF32-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-UTF32-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-UTF8-H \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-UTF8-V \
src/modules/pdf/mupdf-1.6/resources/cmaps/korea/UniKS-X

font_base14_files = \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/Dingbats.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusMon-Bol.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusMon-BolObl.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusMon-Obl.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusMon-Reg.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusRom-Ita.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusRom-Med.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusRom-MedIta.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusRom-Reg.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusSan-Bol.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusSan-BolIta.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusSan-Ita.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/NimbusSan-Reg.cff \
src/modules/pdf/mupdf-1.6/resources/fonts/urw/StandardSymL.cff

font_droid_files = \
src/modules/pdf/mupdf-1.6/resources/fonts/droid/DroidSans.ttf \
src/modules/pdf/mupdf-1.6/resources/fonts/droid/DroidSansMono.ttf

font_cjk_files = \
src/modules/pdf/mupdf-1.6/resources/fonts/droid/DroidSansFallback.ttc

font_cjk_full_files = \
src/modules/pdf/mupdf-1.6/resources/fonts/droid/DroidSansFallbackFull.ttc

src/modules/pdf/mupdf-1.6/generated:
	@echo "  MKDIR  generated/"
	@$(MKDIR_P) src/modules/pdf/mupdf-1.6/generated/

src/modules/pdf/mupdf-1.6/generated/gen_cmap_cns.h: src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) src/modules/pdf/mupdf-1.6/generated $(cmap_cns_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) $@ $(cmap_cns_files)

src/modules/pdf/mupdf-1.6/generated/gen_cmap_gb.h: src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) src/modules/pdf/mupdf-1.6/generated $(cmap_gb_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) $@ $(cmap_gb_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.6/generated/gen_cmap_japan.h: src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) src/modules/pdf/mupdf-1.6/generated $(cmap_japan_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) $@ $(cmap_japan_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.6/generated/gen_cmap_korea.h: src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) src/modules/pdf/mupdf-1.6/generated $(cmap_korea_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/cmapdump$(EXEEXT) $@ $(cmap_korea_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.6/generated/gen_font_base14.h: src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) src/modules/pdf/mupdf-1.6/generated $(font_base14_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) $@ $(font_base14_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.6/generated/gen_font_droid.h: src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) src/modules/pdf/mupdf-1.6/generated $(font_droid_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) $@ $(font_droid_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.6/generated/gen_font_cjk.h: src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) src/modules/pdf/mupdf-1.6/generated $(font_cjk_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) $@ $(font_cjk_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.6/generated/gen_font_cjk_full.h: src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) src/modules/pdf/mupdf-1.6/generated $(font_cjk_files)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/fontdump$(EXEEXT) $@ $(font_cjk_full_files) > /dev/null 2>&1

src/modules/pdf/mupdf-1.6/generated/gen_js_util.h: src/modules/pdf/mupdf-1.6/source/pdf/js/pdf-util.js src/modules/pdf/mupdf-1.6/generated src/modules/pdf/mupdf-1.6/scripts/cquote$(EXEEXT)
	@echo "  GEN    $@"
	@src/modules/pdf/mupdf-1.6/scripts/cquote$(EXEEXT) $@ src/modules/pdf/mupdf-1.6/source/pdf/js/pdf-util.js > /dev/null 2>&1

JS_SOURCES = \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsarray.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsboolean.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsbuiltin.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jscompile.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsdate.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsdtoa.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsdump.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jserror.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsfunction.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsgc.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsintern.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jslex.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsmath.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsnumber.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsobject.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/json.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsparse.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsproperty.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsregexp.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsrun.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsstate.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsstring.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/jsvalue.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/regex.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/utf.c \
src/modules/pdf/mupdf-1.6/thirdparty/mujs/utftype.c \
src/modules/pdf/mupdf-1.6/source/pdf/js/pdf-js.c \
src/modules/pdf/mupdf-1.6/source/pdf/js/pdf-jsimp-mu.c \
src/modules/pdf/mupdf-1.6/source/pdf/js/pdf-util.js

FITZ_SOURCES = \
src/modules/pdf/mupdf-1.6/source/img/muimage.c \
src/modules/pdf/mupdf-1.6/source/tiff/mutiff.c \
src/modules/pdf/mupdf-1.6/source/fitz/bbox-device.c \
src/modules/pdf/mupdf-1.6/source/fitz/bitmap.c \
src/modules/pdf/mupdf-1.6/source/fitz/buffer.c \
src/modules/pdf/mupdf-1.6/source/fitz/colorspace.c \
src/modules/pdf/mupdf-1.6/source/fitz/compressed-buffer.c \
src/modules/pdf/mupdf-1.6/source/fitz/context.c \
src/modules/pdf/mupdf-1.6/source/fitz/crypt-aes.c \
src/modules/pdf/mupdf-1.6/source/fitz/crypt-arc4.c \
src/modules/pdf/mupdf-1.6/source/fitz/crypt-md5.c \
src/modules/pdf/mupdf-1.6/source/fitz/crypt-sha2.c \
src/modules/pdf/mupdf-1.6/source/fitz/device.c \
src/modules/pdf/mupdf-1.6/source/fitz/document-all.c \
src/modules/pdf/mupdf-1.6/source/fitz/document.c \
src/modules/pdf/mupdf-1.6/source/fitz/document-no-run.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-affine.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-blend.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-device.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-edge.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-glyph.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-imp.h \
src/modules/pdf/mupdf-1.6/source/fitz/draw-mesh.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-paint.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-path.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-scale-simple.c \
src/modules/pdf/mupdf-1.6/source/fitz/draw-unpack.c \
src/modules/pdf/mupdf-1.6/source/fitz/error.c \
src/modules/pdf/mupdf-1.6/source/fitz/filter-basic.c \
src/modules/pdf/mupdf-1.6/source/fitz/filter-dct.c \
src/modules/pdf/mupdf-1.6/source/fitz/filter-fax.c \
src/modules/pdf/mupdf-1.6/source/fitz/filter-flate.c \
src/modules/pdf/mupdf-1.6/source/fitz/filter-jbig2.c \
src/modules/pdf/mupdf-1.6/source/fitz/filter-leech.c \
src/modules/pdf/mupdf-1.6/source/fitz/filter-lzw.c \
src/modules/pdf/mupdf-1.6/source/fitz/filter-predict.c \
src/modules/pdf/mupdf-1.6/source/fitz/font.c \
src/modules/pdf/mupdf-1.6/source/fitz/ftoa.c \
src/modules/pdf/mupdf-1.6/source/fitz/function.c \
src/modules/pdf/mupdf-1.6/source/fitz/geometry.c \
src/modules/pdf/mupdf-1.6/source/fitz/getopt.c \
src/modules/pdf/mupdf-1.6/source/fitz/glyph.c \
src/modules/pdf/mupdf-1.6/source/fitz/halftone.c \
src/modules/pdf/mupdf-1.6/source/fitz/hash.c \
src/modules/pdf/mupdf-1.6/source/fitz/image.c \
src/modules/pdf/mupdf-1.6/source/fitz/jmemcust.c \
src/modules/pdf/mupdf-1.6/source/fitz/jmemcust.h \
src/modules/pdf/mupdf-1.6/source/fitz/link.c \
src/modules/pdf/mupdf-1.6/source/fitz/list-device.c \
src/modules/pdf/mupdf-1.6/source/fitz/load-jpeg.c \
src/modules/pdf/mupdf-1.6/source/fitz/load-jpx.c \
src/modules/pdf/mupdf-1.6/source/fitz/load-jxr.c \
src/modules/pdf/mupdf-1.6/source/fitz/load-png.c \
src/modules/pdf/mupdf-1.6/source/fitz/load-tiff.c \
src/modules/pdf/mupdf-1.6/source/fitz/memento.c \
src/modules/pdf/mupdf-1.6/source/fitz/memory.c \
src/modules/pdf/mupdf-1.6/source/fitz/outline.c \
src/modules/pdf/mupdf-1.6/source/fitz/output.c \
src/modules/pdf/mupdf-1.6/source/fitz/output-pcl.c \
src/modules/pdf/mupdf-1.6/source/fitz/output-pwg.c \
src/modules/pdf/mupdf-1.6/source/fitz/path.c \
src/modules/pdf/mupdf-1.6/source/fitz/pixmap.c \
src/modules/pdf/mupdf-1.6/source/fitz/printf.c \
src/modules/pdf/mupdf-1.6/source/fitz/shade.c \
src/modules/pdf/mupdf-1.6/source/fitz/stext-device.c \
src/modules/pdf/mupdf-1.6/source/fitz/stext-output.c \
src/modules/pdf/mupdf-1.6/source/fitz/stext-paragraph.c \
src/modules/pdf/mupdf-1.6/source/fitz/stext-search.c \
src/modules/pdf/mupdf-1.6/source/fitz/store.c \
src/modules/pdf/mupdf-1.6/source/fitz/stream-open.c \
src/modules/pdf/mupdf-1.6/source/fitz/stream-prog.c \
src/modules/pdf/mupdf-1.6/source/fitz/stream-read.c \
src/modules/pdf/mupdf-1.6/source/fitz/string.c \
src/modules/pdf/mupdf-1.6/source/fitz/strtod.c \
src/modules/pdf/mupdf-1.6/source/fitz/svg-device.c \
src/modules/pdf/mupdf-1.6/source/fitz/test-device.c \
src/modules/pdf/mupdf-1.6/source/fitz/text.c \
src/modules/pdf/mupdf-1.6/source/fitz/time.c \
src/modules/pdf/mupdf-1.6/source/fitz/trace-device.c \
src/modules/pdf/mupdf-1.6/source/fitz/transition.c \
src/modules/pdf/mupdf-1.6/source/fitz/tree.c \
src/modules/pdf/mupdf-1.6/source/fitz/ucdn.c \
src/modules/pdf/mupdf-1.6/source/fitz/ucdn.h \
src/modules/pdf/mupdf-1.6/source/fitz/unicodedata_db.h \
src/modules/pdf/mupdf-1.6/source/fitz/xml.c

PDF_SOURCES = \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-annot.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-annot-edit.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-appearance.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-clean.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-cmap.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-cmap-load.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-cmap-parse.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-cmap-table.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-colorspace.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-crypt.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-device.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-encoding.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-encodings.h \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-event.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-field.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-font.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-fontfile.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-form.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-function.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-glyphlist.h \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-image.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-interpret.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-interpret-imp.h \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-lex.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-metrics.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-nametree.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-object.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-op-buffer.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-op-filter.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-op-run.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-outline.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-page.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-parse.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-pattern.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-pkcs7.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-repair.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-run.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-shade.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-store.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-stream.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-type3.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-unicode.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-write.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-xobject.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-xref-aux.c \
src/modules/pdf/mupdf-1.6/source/pdf/pdf-xref.c

JBIG2DEC_SOURCES = \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/getopt1.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/getopt.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/getopt.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_arith.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_arith.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_arith_iaid.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_arith_iaid.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_arith_int.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_arith_int.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2dec.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_generic.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_generic.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_halftone.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_halftone.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_huffman.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_huffman.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_hufftab.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_image.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_image.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_image_pbm.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_image_png.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_metadata.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_metadata.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_mmr.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_mmr.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_page.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_priv.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_refinement.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_segment.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_symbol_dict.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_symbol_dict.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_text.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/jbig2_text.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/memcmp.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/os_types.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/sha1.c \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/sha1.h \
src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec/snprintf.c

XPS_SOURCES = \
src/modules/pdf/mupdf-1.6/source/xps/xps-common.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-doc.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-glyphs.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-gradient.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-image.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-outline.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-path.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-resource.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-tile.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-util.c \
src/modules/pdf/mupdf-1.6/source/xps/xps-zip.c

CBZ_SOURCES = \
src/modules/pdf/mupdf-1.6/source/cbz/mucbz.c

if ETUI_BUILD_STATIC_PDF

src_lib_libetui_la_SOURCES += \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h \
$(FITZ_SOURCES) \
$(PDF_SOURCES) \
$(JBIG2DEC_SOURCES) \
$(XPS_SOURCES) \
$(CBZ_SOURCES) \
$(JS_SOURCES)

src_lib_libetui_la_CPPFLAGS += \
-I$(top_srcdir)/src/lib \
$(BASE_INCLUDES) \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/generated \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/thirdparty/jpeg \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/thirdparty/mujs \
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
$(PDF_SOURCES) \
$(JBIG2DEC_SOURCES) \
$(XPS_SOURCES) \
$(CBZ_SOURCES) \
$(JS_SOURCES)

src_modules_pdf_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
$(BASE_INCLUDES) \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/generated \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/thirdparty/jbig2dec \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/thirdparty/jpeg \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.6/thirdparty/mujs \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@MUPDF_CFLAGS@

src_modules_pdf_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_pdf_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@MUPDF_LIBS@

src_modules_pdf_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_pdf_module_la_LIBTOOLFLAGS = --tag=disable-static

endif

ETUI_CLEANFILES += src/modules/pdf/mupdf-1.6/generated

EXTRA_DIST += \
$(cmap_cns_files) \
$(cmap_gb_files) \
$(cmap_japan_files) \
$(cmap_korea_files) \
$(font_base_files) \
$(font_droid_files) \
$(font_cjk_files)
