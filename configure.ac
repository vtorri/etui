
m4_define([v_maj], [0])
m4_define([v_min], [0])
m4_define([v_mic], [4])
m4_define([v_ver], [v_maj.v_min.v_mic])

m4_define([lt_cur], [m4_eval(v_maj + v_min)])
m4_define([lt_rev], [v_mic])
m4_define([lt_age], [v_min])

AC_INIT([etui], [v_ver], [vincent.torri.remove@gmail.com])
AC_PREREQ([2.52])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.6 dist-xz])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_PROG_AR
LT_INIT([win32-dll disable-static])

version_info="lt_cur:lt_rev:lt_age"
release_info="v_rel"
AC_SUBST([version_info])
AC_SUBST([release_info])

VMAJ=v_maj
AC_SUBST([VMAJ])


### Additional options to configure

AC_ARG_ENABLE([gpl],
   [AS_HELP_STRING([--enable-gpl],
                   [switch license from LGPL v2 to GPL v2 @<:@default=no@:>@])],
   [
    if test "x${enableval}" = "xyes" ; then
       enable_gpl="yes"
    else
       enable_gpl="no"
    fi
   ],
   [enable_gpl="no"])


### Default options with respect to host

AC_CANONICAL_HOST

have_win32="no"
case "$host_os" in
   mingw*)
      have_win32="yes"
   ;;
esac

AM_CONDITIONAL([HAVE_WIN32], [test "x${have_win32}" = "xyes"])

MODULE_ARCH="$host_os-$host_cpu-v_maj.v_min.v_mic"
AC_DEFINE_UNQUOTED([MODULE_ARCH], ["${MODULE_ARCH}"], ["Module architecture"])
AC_SUBST([MODULE_ARCH])

requirements_etui_libs=""
AC_SUBST([requirements_etui_libs])

efl_version="1.18.0"

requirements_etui_pc="eio >= ${efl_version} ecore >= ${efl_version} evas >= ${efl_version} eina >= ${efl_version}"
AC_SUBST([requirements_etui_pc])

requirements_etui_bin_pc="elementary >= ${efl_version} ecore-input >= ${efl_version} ecore-evas >= ${efl_version} ecore >= ${efl_version} evas >= ${efl_version} eina >= ${efl_version}"

want_cb="yes"
want_djvu="yes"
want_epub="yes"
want_pdf="yes"
want_postscript="yes"
want_tiff="yes"
want_txt="yes"


### Checks for programs

AC_PROG_CC

PKG_PROG_PKG_CONFIG

EFL_WITH_BIN([edje], [edje-cc], [edje_cc])


### Checks for libraries

have_no_gpl="no"

ETUI_CHECK_MODULE([CB], [${want_cb}])
ETUI_CHECK_MODULE([DJVU], [${want_djvu}])
ETUI_CHECK_MODULE([EPUB], [${want_epub}])
ETUI_CHECK_MODULE([PDF], [${want_pdf}])
ETUI_CHECK_MODULE([PS], [${want_postscript}])
ETUI_CHECK_MODULE([TIFF], [${want_tiff}])

PKG_CHECK_MODULES([ETUI], [${requirements_etui_pc}])
PKG_CHECK_MODULES([ETUI_BIN], [${requirements_etui_bin_pc}])


### Checks for header files


### Checks for types


### Checks for structures


### Checks for compiler characteristics

AC_C_BIGENDIAN
AM_PROG_CC_C_O

EFL_CHECK_C_COMPILER_FLAGS([ETUI_LIB], [-Wall -Wextra -Wshadow -Wdeclaration-after-statement -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wno-missing-field-initializers -fvisibility=hidden -fdata-sections -ffunction-sections])

EFL_CHECK_C_COMPILER_FLAGS([ETUI_BIN], [-Wall -Wextra -Wshadow -Wdeclaration-after-statement -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wno-missing-field-initializers -fvisibility=hidden -fdata-sections -ffunction-sections])


### Checks for linker characteristics


### Checks for library functions


### Checks for system services

AC_SYS_LARGEFILE

AC_CONFIG_FILES([
Makefile
])

AC_OUTPUT

echo
echo "$PACKAGE $VERSION"
echo
echo "Configuration Options Summary:"
echo
echo "  OS...................: ${host_os}"
echo
echo "  Modules..............:"
echo "    Comic Book.........: ${enable_cb}"
echo "      Comic Book CBZ...: ${have_pkg_libarchive}"
echo "      Comic Book CBR...: ${have_pkg_libarchive}"
echo "      Comic Book CB7...: ${have_pkg_libarchive}"
echo "      Comic Book CBT...: ${have_pkg_libarchive}"
echo "      Comic Book CBA...: no"
if test "x${have_no_gpl}" = "xyes" ; then
echo "    DjVu...............: no (pass --enable-gpl to enable Djvu module)"
else
echo "    DjVu...............: ${enable_djvu}"
fi
echo "    Epub...............: ${enable_epub}"
echo "    PDF................: ${enable_pdf}"
echo "    Postscript.........: ${enable_ps}"
echo "    Tiff...............: ${enable_tiff}"
echo "    Txt................: ${enable_txt}"
echo
echo "Compilation............: make (or gmake)"
echo "  CPPFLAGS.............: $CPPFLAGS"
echo "  CFLAGS...............: $CFLAGS"
echo "  LDFLAGS..............: $LDFLAGS"
echo
echo "Installation...........: make install (as root if needed, with 'su' or 'sudo')"
echo "  prefix...............: $prefix"
echo
