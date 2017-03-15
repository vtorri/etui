
edje_process = \
$(edje) \
-id $(top_srcdir)/data/themes/images \
-id $(top_srcdir)/data/icons/384x384 \
-fd $(top_srcdir)/data/fonts \
-sd $(top_srcdir)/data/themes/sounds \
$< $@ || rm -f $@

edje = @edje_cc@
edje_verbose = $(edje_verbose_@AM_V@)
edje_verbose_ = $(edje_verbose_@AM_DEFAULT_V@)
edje_verbose_0 = @echo "  EDJ     " $@;

data/themes/default.edj: data/themes/default.edc $(top_srcdir)/data/themes/Makefile.mk
	$(AM_V_at)rm -f $@
	$(edje_verbose)$(edje_process)

themesdir = $(pkgdatadir)/themes
themes_DATA = data/themes/default.edj

EXTRA_DIST += \
data/themes/default.edc \
bg_glint.png \
bg_glow_in.png \
bg_shadow.png \
bg_shadow2.png \
bg_shadow3.png \
bg_shine.png \
bg_shine2.png \
bg_shine3.png \
bg_shine4.png \
bg_shine5.png


ETUI_CLEANFILES += default.edj
