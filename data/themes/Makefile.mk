
edje_process = \
$(edje) \
-id $(top_srcdir)/data/themes/images \
-fd $(top_srcdir)/data/fonts \
-sd $(top_srcdir)/data/themes/sounds \
$< $@ || rm -f $@

edje = @edje_cc@
edje_verbose = $(edje_verbose_@AM_V@)
edje_verbose_ = $(edje_verbose_@AM_DEFAULT_V@)
edje_verbose_0 = @echo "  EDJ     " $@;

data/themes/default.edj: $(top_srcdir)/data/themes/default.edc Makefile
	$(AM_V_at)rm -f $@
	$(edje_verbose)$(edje_process)

themesdir = $(pkgdatadir)/themes
themes_DATA = data/themes/default.edj

EXTRA_DIST += \
default.edc \
images/bg_shadow.png \
images/bg_shadow2.png \
images/bg_shadow3.png

ETUI_CLEANFILES += default.edj
