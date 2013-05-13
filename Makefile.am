AUTOMAKE_OPTIONS = subdir-objects

ACLOCAL_AMFLAGS = -I m4

MAINTAINERCLEANFILES = \
Makefile.in \
aclocal.m4 \
compile \
config.guess \
config.h.in \
config.sub \
configure \
depcomp \
install-sh \
ltmain.sh \
missing \
$(DIST_ARCHIVES) \
m4/libtool.m4 \
m4/lt~obsolete.m4 \
m4/ltoptions.m4 \
m4/ltsugar.m4 \
m4/ltversion.m4


EXTRA_DIST =
ETUI_CLEANFILES =

include src/lib/Makefile.mk
include src/bin/Makefile.mk
if ETUI_BUILD_PDF
include src/modules/pdf/Makefile.mk
endif

sed_process = \
$(SED) \
-e 's,@VERSION\@,$(VERSION),g' \
-e 's,@VMAJ\@,$(VMAJ),g' \
-e 's,@prefix\@,$(prefix),g' \
-e 's,@exec_prefix\@,$(exec_prefix),g' \
-e 's,@libdir\@,$(libdir),g' \
-e 's,@includedir\@,$(includedir),g' \
-e 's,@pkgincludedir\@,$(pkgincludedir),g' \
-e 's,@requirements_etui_pc\@,$(requirements_etui_pc),g' \
-e 's,@requirements_etui_libs\@,$(requirements_etui_libs),g' \
< $< > $@ || rm $@

pc_verbose = $(pc_verbose_@AM_V@)
pc_verbose_ = $(pc_verbose_@AM_DEFAULT_V@)
pc_verbose_0 = @echo "  PC      " $@;

etui.pc: $(top_srcdir)/etui.pc.in Makefile
	@rm -f $@
	$(pc_verbose)$(sed_process)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = etui.pc

EXTRA_DIST += \
autogen.sh \
etui.pc.in

ETUI_CLEANFILES += etui.pc

clean-local:
	rm -rf $(ETUI_CLEANFILES)
