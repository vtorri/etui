
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
data/themes/images/bg_glint.png \
data/themes/images/bg_glow_in.png \
data/themes/images/bg_led.png \
data/themes/images/bg_led_base.png \
data/themes/images/bg_led_glow.png \
data/themes/images/bg_led_strobe.png \
data/themes/images/bg_shadow.png \
data/themes/images/bg_shadow2.png \
data/themes/images/bg_shadow3.png \
data/themes/images/bg_shine.png \
data/themes/images/bg_shine2.png \
data/themes/images/bg_shine3.png \
data/themes/images/bg_shine4.png \
data/themes/images/bg_shine5.png \
data/themes/sounds/bell.wav


ETUI_CLEANFILES += default.edj
