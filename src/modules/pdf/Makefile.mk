
GENERATED = src/modules/pdf/mupdf-1.10a/generated

src/modules/pdf/mupdf-1.10a/generated:
	@echo "  MKDIR    generated/"
	@$(MKDIR_P) $@

ETUI_CLEANFILES += \
src/modules/pdf/mupdf-1.10a/generated/

src/modules/pdf/src_modules_pdf_module_la-etui_module_pdf.$(OBJEXT): src/modules/pdf/mupdf-1.10a/include/mupdf/pdf/name-table.h

src/modules/pdf/mupdf-1.10a/source/pdf/.libs/src_modules_pdf_module_la-pdf-cmap-table.o: src/modules/pdf/mupdf-1.10a/generated/gen_cmap_cns.h src/modules/pdf/mupdf-1.10a/generated/gen_cmap_gb.h src/modules/pdf/mupdf-1.10a/generated/gen_cmap_japan.h src/modules/pdf/mupdf-1.10a/generated/gen_cmap_korea.h

src/modules/pdf/mupdf-1.10a/source/pdf/.libs/src_modules_pdf_module_la-pdf-js.o: src/modules/pdf/mupdf-1.10a/generated/gen_js_util.h

## namedump

noinst_PROGRAMS = \
src/modules/pdf/mupdf-1.10a/scripts/namedump$(EXEEXT)

src_modules_pdf_mupdf_1_10a_scripts_namedump_SOURCES = src/modules/pdf/mupdf-1.10a/scripts/namedump.c

nodist_src_modules_pdf_module_la_SOURCES = \
src/modules/pdf/mupdf-1.10a/include/mupdf/pdf/name-table.h \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-name-table.h

src/modules/pdf/mupdf-1.10a/include/mupdf/pdf/name-table.h: src/modules/pdf/mupdf-1.10a/scripts/namedump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/namedump$(EXEEXT) src/modules/pdf/mupdf-1.10a/resources/pdf/names.txt src/modules/pdf/mupdf-1.10a/include/mupdf/pdf/name-table.h src/modules/pdf/mupdf-1.10a/source/pdf/pdf-name-table.h

ETUI_CLEANFILES += \
src/modules/pdf/mupdf-1.10a/scripts/namedump$(EXEEXT) \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-name-table.h \
src/modules/pdf/mupdf-1.10a/include/mupdf/pdf/name-table.h

## cmapdump

noinst_PROGRAMS += \
src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT)

src_modules_pdf_mupdf_1_10a_scripts_cmapdump_SOURCES = src/modules/pdf/mupdf-1.10a/scripts/cmapdump.c

src_modules_pdf_mupdf_1_10a_scripts_cmapdump_CPPFLAGS = \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/include

CMAP_CNS_SRC = \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/Adobe-CNS1-0 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/Adobe-CNS1-1 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/Adobe-CNS1-2 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/Adobe-CNS1-3 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/Adobe-CNS1-4 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/Adobe-CNS1-5 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/Adobe-CNS1-6 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/Adobe-CNS1-UCS2 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/B5pc-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/B5pc-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/CNS1-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/CNS1-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/CNS2-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/CNS2-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/CNS-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/CNS-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/ETen-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/ETen-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/ETenms-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/ETenms-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/ETHK-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/ETHK-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKdla-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKdla-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKdlb-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKdlb-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKgccs-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKgccs-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKm314-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKm314-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKm471-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKm471-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKscs-B5-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/HKscs-B5-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-UCS2-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-UCS2-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-UTF16-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-UTF16-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-UTF32-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-UTF32-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-UTF8-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-UTF8-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/cns/UniCNS-X

CMAP_GB_SRC = \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/Adobe-GB1-0 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/Adobe-GB1-1 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/Adobe-GB1-2 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/Adobe-GB1-3 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/Adobe-GB1-4 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/Adobe-GB1-5 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/Adobe-GB1-UCS2 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GB-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GB-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GB-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBK2K-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBK2K-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBK-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBK-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBKp-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBKp-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBpc-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBpc-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBT-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBT-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBT-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBTpc-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBTpc-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GBT-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/GB-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-UCS2-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-UCS2-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-UTF16-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-UTF16-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-UTF32-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-UTF32-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-UTF8-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-UTF8-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/gb/UniGB-X

CMAP_JAPAN_SRC = \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/78-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/78-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/78-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/78ms-RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/78ms-RKSJ-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/78-RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/78-RKSJ-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/78-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/83pv-RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/90msp-RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/90msp-RKSJ-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/90ms-RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/90ms-RKSJ-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/90pv-RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/90pv-RKSJ-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Add-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Add-RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Add-RKSJ-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Add-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Adobe-Japan1-0 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Adobe-Japan1-1 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Adobe-Japan1-2 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Adobe-Japan1-3 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Adobe-Japan1-4 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Adobe-Japan1-5 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Adobe-Japan1-6 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Adobe-Japan1-UCS2 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Ext-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Ext-RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Ext-RKSJ-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Ext-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Hankaku \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Hiragana \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Katakana \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/NWP-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/NWP-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/RKSJ-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/RKSJ-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/Roman \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS2004-UTF16-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS2004-UTF16-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS2004-UTF32-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS2004-UTF32-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS2004-UTF8-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS2004-UTF8-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJISPro-UCS2-HW-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJISPro-UCS2-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJISPro-UTF8-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UCS2-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UCS2-HW-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UCS2-HW-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UCS2-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UTF16-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UTF16-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UTF32-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UTF32-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UTF8-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-UTF8-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-X \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJISX02132004-UTF32-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJISX02132004-UTF32-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJISX0213-UTF32-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJISX0213-UTF32-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-X16 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-X32 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/UniJIS-X8 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/japan/WP-Symbol

CMAP_KOREA_SRC = \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/Adobe-Korea1-0 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/Adobe-Korea1-1 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/Adobe-Korea1-2 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/Adobe-Korea1-UCS2 \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSC-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSC-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSC-Johab-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSC-Johab-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSCms-UHC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSCms-UHC-HW-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSCms-UHC-HW-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSCms-UHC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSCpc-EUC-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSCpc-EUC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/KSC-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-UCS2-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-UCS2-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-UTF16-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-UTF16-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-UTF32-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-UTF32-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-UTF8-H \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-UTF8-V \
src/modules/pdf/mupdf-1.10a/resources/cmaps/korea/UniKS-X

src/modules/pdf/mupdf-1.10a/generated/gen_cmap_cns.h: src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT) $@ $(CMAP_CNS_SRC)

src/modules/pdf/mupdf-1.10a/generated/gen_cmap_gb.h: src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT) $@ $(CMAP_GB_SRC)

src/modules/pdf/mupdf-1.10a/generated/gen_cmap_japan.h: src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT) $@ $(CMAP_JAPAN_SRC)

src/modules/pdf/mupdf-1.10a/generated/gen_cmap_korea.h: src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT) $@ $(CMAP_KOREA_SRC)

nodist_src_modules_pdf_module_la_SOURCES += \
src/modules/pdf/mupdf-1.10a/generated/gen_cmap_cns.h \
src/modules/pdf/mupdf-1.10a/generated/gen_cmap_gb.h \
src/modules/pdf/mupdf-1.10a/generated/gen_cmap_japan.h \
src/modules/pdf/mupdf-1.10a/generated/gen_cmap_korea.h

ETUI_CLEANFILES += \
src/modules/pdf/mupdf-1.10a/scripts/cmapdump$(EXEEXT)

## fontdump

noinst_PROGRAMS += \
src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)

src_modules_pdf_mupdf_1_10a_scripts_fontdump_SOURCES = src/modules/pdf/mupdf-1.10a/scripts/fontdump.c

font_droid_files = \
src/modules/pdf/mupdf-1.10a/resources/fonts/droid/DroidSansFallback.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/droid/DroidSansFallbackFull.ttf

font_han_files = \
src/modules/pdf/mupdf-1.10a/resources/fonts/han/SourceHanSansCN-Regular.otf \
src/modules/pdf/mupdf-1.10a/resources/fonts/han/SourceHanSansJP-Regular.otf \
src/modules/pdf/mupdf-1.10a/resources/fonts/han/SourceHanSansKR-Regular.otf \
src/modules/pdf/mupdf-1.10a/resources/fonts/han/SourceHanSansTW-Regular.otf

font_noto_files = \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoEmoji-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoKufiArabic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoNaskhArabic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoNastaliqUrdu-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansArmenian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansAvestan-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBalinese-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBamum-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBatak-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBengali-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBrahmi-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBuginese-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBuhid-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCanadianAboriginal-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCarian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCham-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCherokee-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCoptic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCuneiform-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCypriot-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansDeseret-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansDevanagari-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansEgyptianHieroglyphs-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansEthiopic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGeorgian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGlagolitic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGothic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGujarati-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGurmukhi-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansHanunoo-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansHebrew-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansImperialAramaic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansInscriptionalPahlavi-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansInscriptionalParthia-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansJavanese-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKaithi-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKannada-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKayahLi-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKharoshthi-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKhmer-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLao-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLepcha-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLimbu-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLinearB-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLisu-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLycian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLydian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMalayalam-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMandaic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMeeteiMayek-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMongolian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMyanmar-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansNewTaiLue-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansNKo-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOgham-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOlChiki-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOldItalic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOldPersian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOldSouthArabian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOldTurkic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOriya-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOsmanya-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansPhagsPa-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansPhoenician-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSans-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansRejang-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansRunic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSamaritan-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSaurashtra-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansShavian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSinhala-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSundanese-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSylotiNagri-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSymbols-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSyriacEastern-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSyriacEstrangela-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSyriacWestern-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTagalog-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTagbanwa-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTaiLe-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTaiTham-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTaiViet-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTamil-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTelugu-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansThaana-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansThai-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTibetan-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTifinagh-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansUgaritic-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansVai-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansYi-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifArmenian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifBengali-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifDevanagari-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifGeorgian-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifGujarati-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifKannada-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifKhmer-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifLao-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifMalayalam-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerif-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifTamil-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifTelugu-Regular.ttf \
src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifThai-Regular.ttf

font_sil_files = \
src/modules/pdf/mupdf-1.10a/resources/fonts/sil/CharisSIL-B.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/sil/CharisSIL-BI.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/sil/CharisSIL-I.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/sil/CharisSIL-R.cff

font_urw_files = \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/Dingbats.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusMonoPS-Bold.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusMonoPS-BoldItalic.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusMonoPS-Italic.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusMonoPS-Regular.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusRoman-Bold.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusRoman-BoldItalic.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusRoman-Italic.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusRoman-Regular.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusSans-Bold.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusSans-BoldOblique.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusSans-Oblique.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusSans-Regular.cff \
src/modules/pdf/mupdf-1.10a/resources/fonts/urw/StandardSymbolsPS.cff

# droid

src/modules/pdf/mupdf-1.10a/generated/DroidSansFallback.c: src/modules/pdf/mupdf-1.10a/resources/fonts/droid/DroidSansFallback.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/DroidSansFallbackFull.c: src/modules/pdf/mupdf-1.10a/resources/fonts/droid/DroidSansFallbackFull.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

# han

src/modules/pdf/mupdf-1.10a/generated/SourceHanSansCN-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/han/SourceHanSansCN-Regular.otf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/SourceHanSansJP-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/han/SourceHanSansJP-Regular.otf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/SourceHanSansKR-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/han/SourceHanSansKR-Regular.otf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/SourceHanSansTW-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/han/SourceHanSansTW-Regular.otf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

# noto

src/modules/pdf/mupdf-1.10a/generated/NotoEmoji-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoEmoji-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoKufiArabic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoKufiArabic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoNaskhArabic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoNaskhArabic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoNastaliqUrdu-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoNastaliqUrdu-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansArmenian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansArmenian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansAvestan-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansAvestan-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansBalinese-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBalinese-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansBamum-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBamum-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansBatak-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBatak-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansBengali-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBengali-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansBrahmi-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBrahmi-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansBuginese-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBuginese-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansBuhid-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansBuhid-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansCanadianAboriginal-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCanadianAboriginal-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansCarian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCarian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansCham-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCham-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansCherokee-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCherokee-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansCoptic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCoptic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansCuneiform-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCuneiform-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansCypriot-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansCypriot-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansDeseret-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansDeseret-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansDevanagari-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansDevanagari-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansEgyptianHieroglyphs-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansEgyptianHieroglyphs-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansEthiopic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansEthiopic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansGeorgian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGeorgian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansGlagolitic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGlagolitic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansGothic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGothic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansGujarati-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGujarati-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansGurmukhi-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansGurmukhi-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansHanunoo-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansHanunoo-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansHebrew-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansHebrew-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansImperialAramaic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansImperialAramaic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansInscriptionalPahlavi-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansInscriptionalPahlavi-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansInscriptionalParthian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansInscriptionalParthia-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansJavanese-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansJavanese-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansKaithi-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKaithi-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansKannada-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKannada-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansKayahLi-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKayahLi-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansKharoshthi-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKharoshthi-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansKhmer-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansKhmer-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansLao-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLao-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansLepcha-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLepcha-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansLimbu-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLimbu-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansLinearB-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLinearB-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansLisu-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLisu-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansLycian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLycian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansLydian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansLydian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansMalayalam-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMalayalam-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansMandaic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMandaic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansMeeteiMayek-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMeeteiMayek-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansMongolian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMongolian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansMyanmar-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansMyanmar-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansNewTaiLue-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansNewTaiLue-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansNKo-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansNKo-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansOgham-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOgham-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansOlChiki-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOlChiki-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansOldItalic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOldItalic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansOldPersian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOldPersian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansOldSouthArabian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOldSouthArabian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansOldTurkic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOldTurkic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansOriya-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOriya-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansOsmanya-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansOsmanya-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansPhagsPa-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansPhagsPa-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansPhoenician-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansPhoenician-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSans-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSans-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansRejang-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansRejang-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansRunic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansRunic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSamaritan-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSamaritan-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSaurashtra-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSaurashtra-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansShavian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansShavian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSinhala-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSinhala-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSundanese-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSundanese-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSylotiNagri-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSylotiNagri-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSymbols-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSymbols-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSyriacEastern-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSyriacEastern-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSyriacEstrangela-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSyriacEstrangela-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansSyriacWestern-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansSyriacWestern-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTagalog-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTagalog-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTagbanwa-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTagbanwa-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTaiLe-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTaiLe-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTaiTham-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTaiTham-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTaiViet-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTaiViet-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTamil-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTamil-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTelugu-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTelugu-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansThaana-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansThaana-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansThai-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansThai-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTibetan-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTibetan-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansTifinagh-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansTifinagh-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansUgaritic-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansUgaritic-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansVai-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansVai-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSansYi-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSansYi-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifArmenian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifArmenian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifBengali-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifBengali-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifDevanagari-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifDevanagari-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifGeorgian-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifGeorgian-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifGujarati-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifGujarati-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifKannada-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifKannada-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifKhmer-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifKhmer-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifLao-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifLao-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifMalayalam-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifMalayalam-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerif-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerif-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifTamil-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifTamil-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifTelugu-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifTelugu-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NotoSerifThai-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/noto/NotoSerifThai-Regular.ttf src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

# sil

src/modules/pdf/mupdf-1.10a/generated/CharisSIL-B.c: src/modules/pdf/mupdf-1.10a/resources/fonts/sil/CharisSIL-B.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/CharisSIL-BI.c: src/modules/pdf/mupdf-1.10a/resources/fonts/sil/CharisSIL-BI.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/CharisSIL-I.c: src/modules/pdf/mupdf-1.10a/resources/fonts/sil/CharisSIL-I.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/CharisSIL-R.c: src/modules/pdf/mupdf-1.10a/resources/fonts/sil/CharisSIL-R.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

# urw

src/modules/pdf/mupdf-1.10a/generated/Dingbats.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/Dingbats.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusMonoPS-Bold.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusMonoPS-Bold.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusMonoPS-BoldItalic.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusMonoPS-BoldItalic.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusMonoPS-Italic.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusMonoPS-Italic.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusMonoPS-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusMonoPS-Regular.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusRoman-Bold.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusRoman-Bold.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusRoman-BoldItalic.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusRoman-BoldItalic.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusRoman-Italic.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusRoman-Italic.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusRoman-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusRoman-Regular.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusSans-Bold.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusSans-Bold.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusSans-BoldOblique.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusSans-BoldOblique.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusSans-Oblique.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusSans-Oblique.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/NimbusSans-Regular.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/NimbusSans-Regular.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

src/modules/pdf/mupdf-1.10a/generated/StandardSymbolsPS.c: src/modules/pdf/mupdf-1.10a/resources/fonts/urw/StandardSymbolsPS.cff src/modules/pdf/mupdf-1.10a/generated src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT) $@ $<

ETUI_CLEANFILES += \
src/modules/pdf/mupdf-1.10a/scripts/fontdump$(EXEEXT)

## cquote

noinst_PROGRAMS += \
src/modules/pdf/mupdf-1.10a/scripts/cquote$(EXEEXT)

src_modules_pdf_mupdf_1_10a_scripts_cquote_SOURCES = src/modules/pdf/mupdf-1.10a/scripts/cquote.c

src/modules/pdf/mupdf-1.10a/generated/gen_js_util.h: src/modules/pdf/mupdf-1.10a/source/pdf/pdf-js-util.js src/modules/pdf/mupdf-1.10a/scripts/cquote$(EXEEXT)
	@echo "  GEN      $@"
	@src/modules/pdf/mupdf-1.10a/scripts/cquote$(EXEEXT) $@ $<

ETUI_CLEANFILES += \
src/modules/pdf/mupdf-1.10a/scripts/cquote$(EXEEXT)

MUPDF_SOURCES = \
src/modules/pdf/mupdf-1.10a/source/fitz/archive.c \
src/modules/pdf/mupdf-1.10a/source/fitz/bbox-device.c \
src/modules/pdf/mupdf-1.10a/source/fitz/bidi.c \
src/modules/pdf/mupdf-1.10a/source/fitz/bidi-std.c \
src/modules/pdf/mupdf-1.10a/source/fitz/bitmap.c \
src/modules/pdf/mupdf-1.10a/source/fitz/buffer.c \
src/modules/pdf/mupdf-1.10a/source/fitz/colorspace.c \
src/modules/pdf/mupdf-1.10a/source/fitz/compressed-buffer.c \
src/modules/pdf/mupdf-1.10a/source/fitz/context.c \
src/modules/pdf/mupdf-1.10a/source/fitz/crypt-aes.c \
src/modules/pdf/mupdf-1.10a/source/fitz/crypt-arc4.c \
src/modules/pdf/mupdf-1.10a/source/fitz/crypt-md5.c \
src/modules/pdf/mupdf-1.10a/source/fitz/crypt-sha2.c \
src/modules/pdf/mupdf-1.10a/source/fitz/device.c \
src/modules/pdf/mupdf-1.10a/source/fitz/directory.c \
src/modules/pdf/mupdf-1.10a/source/fitz/document.c \
src/modules/pdf/mupdf-1.10a/source/fitz/document-all.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-affine.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-blend.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-device.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-edge.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-glyph.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-mesh.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-paint.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-path.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-scale-simple.c \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-unpack.c \
src/modules/pdf/mupdf-1.10a/source/fitz/error.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-basic.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-dct.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-fax.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-flate.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-jbig2.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-leech.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-lzw.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-predict.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-sgi.c \
src/modules/pdf/mupdf-1.10a/source/fitz/filter-thunder.c \
src/modules/pdf/mupdf-1.10a/source/fitz/font.c \
src/modules/pdf/mupdf-1.10a/source/fitz/ftoa.c \
src/modules/pdf/mupdf-1.10a/source/fitz/function.c \
src/modules/pdf/mupdf-1.10a/source/fitz/geometry.c \
src/modules/pdf/mupdf-1.10a/source/fitz/getopt.c \
src/modules/pdf/mupdf-1.10a/source/fitz/glyph.c \
src/modules/pdf/mupdf-1.10a/source/fitz/halftone.c \
src/modules/pdf/mupdf-1.10a/source/fitz/harfbuzz.c \
src/modules/pdf/mupdf-1.10a/source/fitz/hash.c \
src/modules/pdf/mupdf-1.10a/source/fitz/image.c \
src/modules/pdf/mupdf-1.10a/source/fitz/jmemcust.c \
src/modules/pdf/mupdf-1.10a/source/fitz/link.c \
src/modules/pdf/mupdf-1.10a/source/fitz/list-device.c \
src/modules/pdf/mupdf-1.10a/source/fitz/load-bmp.c \
src/modules/pdf/mupdf-1.10a/source/fitz/load-gif.c \
src/modules/pdf/mupdf-1.10a/source/fitz/load-jpeg.c \
src/modules/pdf/mupdf-1.10a/source/fitz/load-jpx.c \
src/modules/pdf/mupdf-1.10a/source/fitz/load-jxr.c \
src/modules/pdf/mupdf-1.10a/source/fitz/load-png.c \
src/modules/pdf/mupdf-1.10a/source/fitz/load-pnm.c \
src/modules/pdf/mupdf-1.10a/source/fitz/load-tiff.c \
src/modules/pdf/mupdf-1.10a/source/fitz/memento.c \
src/modules/pdf/mupdf-1.10a/source/fitz/memory.c \
src/modules/pdf/mupdf-1.10a/source/fitz/noto.c \
src/modules/pdf/mupdf-1.10a/source/fitz/outline.c \
src/modules/pdf/mupdf-1.10a/source/fitz/output.c \
src/modules/pdf/mupdf-1.10a/source/fitz/output-cbz.c \
src/modules/pdf/mupdf-1.10a/source/fitz/output-pcl.c \
src/modules/pdf/mupdf-1.10a/source/fitz/output-png.c \
src/modules/pdf/mupdf-1.10a/source/fitz/output-pnm.c \
src/modules/pdf/mupdf-1.10a/source/fitz/output-ps.c \
src/modules/pdf/mupdf-1.10a/source/fitz/output-pwg.c \
src/modules/pdf/mupdf-1.10a/source/fitz/output-tga.c \
src/modules/pdf/mupdf-1.10a/source/fitz/path.c \
src/modules/pdf/mupdf-1.10a/source/fitz/pixmap.c \
src/modules/pdf/mupdf-1.10a/source/fitz/pool.c \
src/modules/pdf/mupdf-1.10a/source/fitz/printf.c \
src/modules/pdf/mupdf-1.10a/source/fitz/separation.c \
src/modules/pdf/mupdf-1.10a/source/fitz/shade.c \
src/modules/pdf/mupdf-1.10a/source/fitz/stext-device.c \
src/modules/pdf/mupdf-1.10a/source/fitz/stext-output.c \
src/modules/pdf/mupdf-1.10a/source/fitz/stext-paragraph.c \
src/modules/pdf/mupdf-1.10a/source/fitz/stext-search.c \
src/modules/pdf/mupdf-1.10a/source/fitz/store.c \
src/modules/pdf/mupdf-1.10a/source/fitz/stream-open.c \
src/modules/pdf/mupdf-1.10a/source/fitz/stream-prog.c \
src/modules/pdf/mupdf-1.10a/source/fitz/stream-read.c \
src/modules/pdf/mupdf-1.10a/source/fitz/string.c \
src/modules/pdf/mupdf-1.10a/source/fitz/strtod.c \
src/modules/pdf/mupdf-1.10a/source/fitz/strtof.c \
src/modules/pdf/mupdf-1.10a/source/fitz/svg-device.c \
src/modules/pdf/mupdf-1.10a/source/fitz/tempfile.c \
src/modules/pdf/mupdf-1.10a/source/fitz/test-device.c \
src/modules/pdf/mupdf-1.10a/source/fitz/text.c \
src/modules/pdf/mupdf-1.10a/source/fitz/time.c \
src/modules/pdf/mupdf-1.10a/source/fitz/trace-device.c \
src/modules/pdf/mupdf-1.10a/source/fitz/track-usage.c \
src/modules/pdf/mupdf-1.10a/source/fitz/transition.c \
src/modules/pdf/mupdf-1.10a/source/fitz/tree.c \
src/modules/pdf/mupdf-1.10a/source/fitz/ucdn.c \
src/modules/pdf/mupdf-1.10a/source/fitz/untar.c \
src/modules/pdf/mupdf-1.10a/source/fitz/unzip.c \
src/modules/pdf/mupdf-1.10a/source/fitz/util.c \
src/modules/pdf/mupdf-1.10a/source/fitz/writer.c \
src/modules/pdf/mupdf-1.10a/source/fitz/xml.c \
src/modules/pdf/mupdf-1.10a/source/fitz/zip.c \
src/modules/pdf/mupdf-1.10a/source/fitz/bidi-imp.h \
src/modules/pdf/mupdf-1.10a/source/fitz/colorspace-imp.h \
src/modules/pdf/mupdf-1.10a/source/fitz/draw-imp.h \
src/modules/pdf/mupdf-1.10a/source/fitz/fitz-imp.h \
src/modules/pdf/mupdf-1.10a/source/fitz/font-imp.h \
src/modules/pdf/mupdf-1.10a/source/fitz/glyph-cache-imp.h \
src/modules/pdf/mupdf-1.10a/source/fitz/jmemcust.h \
src/modules/pdf/mupdf-1.10a/source/fitz/paint-glyph.h \
src/modules/pdf/mupdf-1.10a/source/fitz/unicodedata_db.h


MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-annot.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-annot-edit.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-appearance.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-clean.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-clean-file.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-cmap.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-cmap-load.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-cmap-parse.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-cmap-table.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-colorspace.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-crypt.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-device.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-encoding.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-event.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-field.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-font.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-form.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-function.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-graft.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-image.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-interpret.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-js.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-layer.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-lex.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-metrics.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-nametree.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-object.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-op-buffer.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-op-filter.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-op-run.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-outline.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-page.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-parse.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-pattern.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-pkcs7.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-repair.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-resources.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-run.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-shade.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-store.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-stream.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-type3.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-unicode.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-write.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-xobject.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-xref.c \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-encodings.h \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-glyphlist.h \
src/modules/pdf/mupdf-1.10a/source/pdf/pdf-imp.h

MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/source/xps/xps-common.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-doc.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-glyphs.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-gradient.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-image.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-link.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-outline.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-path.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-resource.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-tile.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-util.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-zip.c \
src/modules/pdf/mupdf-1.10a/source/xps/xps-imp.h

MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/source/svg/svg-color.c \
src/modules/pdf/mupdf-1.10a/source/svg/svg-doc.c \
src/modules/pdf/mupdf-1.10a/source/svg/svg-parse.c \
src/modules/pdf/mupdf-1.10a/source/svg/svg-run.c \
src/modules/pdf/mupdf-1.10a/source/svg/svg-imp.h

MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/source/html/css-apply.c \
src/modules/pdf/mupdf-1.10a/source/html/css-parse.c \
src/modules/pdf/mupdf-1.10a/source/html/epub-doc.c \
src/modules/pdf/mupdf-1.10a/source/html/html-doc.c \
src/modules/pdf/mupdf-1.10a/source/html/html-font.c \
src/modules/pdf/mupdf-1.10a/source/html/html-layout.c

MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/source/gprf/gprf-doc.c \
src/modules/pdf/mupdf-1.10a/source/gprf/gprf-skeleton.c

MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/source/cbz/mucbz.c \
src/modules/pdf/mupdf-1.10a/source/cbz/muimg.c \
src/modules/pdf/mupdf-1.10a/source/cbz/mutiff.c

# jbig2dec

MUPDF_SOURCES += \
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

MUPDF_SOURCES += \
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

MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/thirdparty/mujs/one.c

## generated

# js

MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/generated/gen_js_util.h

# name table

#MUPDF_SOURCES += \
#src/modules/pdf/mupdf-1.10a/source/pdf/pdf-name-table.h

# cmaps

#MUPDF_SOURCES += \
#src/modules/pdf/mupdf-1.10a/generated/gen_cmap_cns.h \
#src/modules/pdf/mupdf-1.10a/generated/gen_cmap_gb.h \
#src/modules/pdf/mupdf-1.10a/generated/gen_cmap_japan.h \
#src/modules/pdf/mupdf-1.10a/generated/gen_cmap_korea.h

#fonts

MUPDF_SOURCES += \
src/modules/pdf/mupdf-1.10a/generated/DroidSansFallback.c \
src/modules/pdf/mupdf-1.10a/generated/DroidSansFallbackFull.c \
src/modules/pdf/mupdf-1.10a/generated/SourceHanSansCN-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/SourceHanSansJP-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/SourceHanSansKR-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/SourceHanSansTW-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoEmoji-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoKufiArabic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoNaskhArabic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoNastaliqUrdu-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansArmenian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansAvestan-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansBalinese-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansBamum-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansBatak-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansBengali-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansBrahmi-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansBuginese-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansBuhid-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansCanadianAboriginal-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansCarian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansCham-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansCherokee-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansCoptic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansCuneiform-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansCypriot-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansDeseret-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansDevanagari-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansEgyptianHieroglyphs-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansEthiopic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansGeorgian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansGlagolitic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansGothic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansGujarati-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansGurmukhi-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansHanunoo-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansHebrew-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansImperialAramaic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansInscriptionalPahlavi-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansInscriptionalParthian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansJavanese-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansKaithi-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansKannada-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansKayahLi-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansKharoshthi-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansKhmer-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansLao-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansLepcha-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansLimbu-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansLinearB-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansLisu-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansLycian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansLydian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansMalayalam-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansMandaic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansMeeteiMayek-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansMongolian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansMyanmar-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansNewTaiLue-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansNKo-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansOgham-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansOlChiki-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansOldItalic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansOldPersian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansOldSouthArabian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansOldTurkic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansOriya-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansOsmanya-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansPhagsPa-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansPhoenician-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSans-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansRejang-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansRunic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSamaritan-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSaurashtra-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansShavian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSinhala-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSundanese-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSylotiNagri-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSymbols-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSyriacEastern-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSyriacEstrangela-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansSyriacWestern-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTagalog-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTagbanwa-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTaiLe-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTaiTham-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTaiViet-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTamil-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTelugu-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansThaana-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansThai-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTibetan-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansTifinagh-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansUgaritic-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansVai-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSansYi-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifArmenian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifBengali-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifDevanagari-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifGeorgian-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifGujarati-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifKannada-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifKhmer-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifLao-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifMalayalam-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerif-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifTamil-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifTelugu-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NotoSerifThai-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/CharisSIL-B.c \
src/modules/pdf/mupdf-1.10a/generated/CharisSIL-BI.c \
src/modules/pdf/mupdf-1.10a/generated/CharisSIL-I.c \
src/modules/pdf/mupdf-1.10a/generated/CharisSIL-R.c \
src/modules/pdf/mupdf-1.10a/generated/Dingbats.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusMonoPS-Bold.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusMonoPS-BoldItalic.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusMonoPS-Italic.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusMonoPS-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusRoman-Bold.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusRoman-BoldItalic.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusRoman-Italic.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusRoman-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusSans-Bold.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusSans-BoldOblique.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusSans-Oblique.c \
src/modules/pdf/mupdf-1.10a/generated/NimbusSans-Regular.c \
src/modules/pdf/mupdf-1.10a/generated/StandardSymbolsPS.c



if ETUI_BUILD_STATIC_PDF

src_lib_libetui_la_SOURCES += \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h \
$(MUPDF_SOURCES)

src_lib_libetui_la_CPPFLAGS += \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/generated \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/include
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/thirdparty/mujs \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/thirdparty/jpeg \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2 \
-DHAVE_STDINT_H \
-DJBIG_EXTERNAL_MEMENTO_H=\"src/modules/pdf/mupdf-1.10a/include/mupdf/memento.h\" \
-DOPJ_STATIC \
-DOPJ_HAVE_STDINT_H \
-DOPJ_HAVE_INTTYPES_H=1 \
-DUSE_JPIP=1 \
@MUPDF_CFLAGS@

src_lib_libetui_la_LIBADD += \
@MUPDF_LIBS@

else

etui_modules_pdfdir = $(pkglibdir)/modules/pdf/$(MODULE_ARCH)
etui_modules_pdf_LTLIBRARIES = src/modules/pdf/module.la

src_modules_pdf_module_la_SOURCES = \
src/modules/pdf/etui_module_pdf.c \
src/modules/pdf/etui_module_pdf.h \
$(MUPDF_SOURCES) \
$(JBIG2DEC_SOURCES)

src_modules_pdf_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/generated \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/include \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/thirdparty/mujs \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/thirdparty/jpeg \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/thirdparty/jbig2dec \
-I$(top_srcdir)/src/modules/pdf/mupdf-1.10a/thirdparty/openjpeg/src/lib/openjp2 \
-DHAVE_STDINT_H \
-DJBIG_EXTERNAL_MEMENTO_H=\"src/modules/pdf/mupdf-1.10a/include/mupdf/memento.h\" \
-DOPJ_STATIC \
-DOPJ_HAVE_STDINT_H \
-DOPJ_HAVE_INTTYPES_H=1 \
-DUSE_JPIP=1 \
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

EXTRA_DIST += \
$(CMAP_CNS_SRC) \
$(CMAP_GB_SRC) \
$(CMAP_JAPAN_SRC) \
$(CMAP_KOREA_SRC) \
$(font_droid_files) \
$(font_han_files) \
$(font_noto_files) \
$(font_sil_files) \
$(font_urw_files)
