
m4_define([v_maj], [0])
m4_define([v_min], [0])
m4_define([v_mic], [1])
m4_define([v_ver], [v_maj.v_min.v_mic])

m4_define([lt_cur], [m4_eval(v_maj + v_min)])
m4_define([lt_rev], [v_mic])
m4_define([lt_age], [v_min])

AC_INIT([etui], [v_ver], [vincent.torri.remove@gmail.com])
AC_PREREQ([2.52])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.6 dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_INIT([win32-dll disable-static])

version_info="lt_cur:lt_rev:lt_age"
release_info="v_rel"
AC_SUBST([version_info])
AC_SUBST([release_info])

VMAJ=v_maj
AC_SUBST([VMAJ])


### Additional options to configure


### Default options with respect to host

AC_CANONICAL_HOST

MODULE_ARCH="$host_os-$host_cpu-v_maj.v_min.v_mic"
AC_DEFINE_UNQUOTED([MODULE_ARCH], ["${MODULE_ARCH}"], ["Module architecture"])
AC_SUBST([MODULE_ARCH])

requirements_etui_libs=""
AC_SUBST([requirements_etui_libs])

efl_version="1.7.0"

requirements_etui_pc="eio >= ${efl_version} ecore >= ${efl_version} evas >= ${efl_version} eina >= ${efl_version}"
AC_SUBST([requirements_etui_pc])

requirements_etui_bin_pc="ecore-input >= ${efl_version} ecore-evas >= ${efl_version} ecore >= ${efl_version} evas >= ${efl_version} eina >= ${efl_version}"

want_img="yes"
want_pdf="yes"
want_postscript="yes"
want_txt="yes"


### Checks for programs

AC_PROG_CC
AC_PROG_CXX
AC_LANG([C])

PKG_PROG_PKG_CONFIG


### Checks for libraries

ETUI_CHECK_MODULE([IMG], [${want_img}])
ETUI_CHECK_MODULE([PDF], [${want_pdf}])
ETUI_CHECK_MODULE([PS], [${want_postscript}])

PKG_CHECK_MODULES([ETUI], [${requirements_etui_pc}])
PKG_CHECK_MODULES([ETUI_BIN], [${requirements_etui_bin_pc}])


### Checks for header files


### Checks for types


### Checks for structures


### Checks for compiler characteristics

AC_C_BIGENDIAN
AM_PROG_CC_C_O

EFL_CHECK_C_COMPILER_FLAGS([ETUI_LIB], [-Wall -Wextra -Wshadow -Wdeclaration-after-statement -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wno-missing-field-initializers -fvisibility=hidden -fdata-sections -ffunction-sections])

EFL_CHECK_CXX_COMPILER_FLAGS([ETUI_LIB], [-Wall -Wextra -Wshadow -Wpointer-arith -Wno-missing-field-initializers -fvisibility=hidden -fdata-sections -ffunction-sections])

EFL_CHECK_C_COMPILER_FLAGS([ETUI_BIN], [-Wall -Wextra -Wshadow -Wdeclaration-after-statement -Wmissing-prototypes -Wstrict-prototypes -Wpointer-arith -Wno-missing-field-initializers -fvisibility=hidden -fdata-sections -ffunction-sections])


### Checks for linker characteristics


### Checks for library functions

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
echo "    Image..............: ${enable_img}"
echo "      Comic Book CBZ...: ${have_pkg_zlib}"
echo "      Comic Book CBR...: yes"
echo "      Comic Book CBA...: auto"
echo "      Comic Book CB7...: "
echo "      Comic Book CBT...: "
echo "    PDF................: ${enable_pdf}"
echo "    Postscript.........: ${enable_ps}"
echo "    Txt................: ${enable_txt}"
echo
echo "Compilation............: make (or gmake)"
echo "  CPPFLAGS.............: $CPPFLAGS"
echo "  CFLAGS...............: $CFLAGS"
echo "  CXXFLAGS.............: $CXXFLAGS"
echo "  LDFLAGS..............: $LDFLAGS"
echo
echo "Installation...........: make install (as root if needed, with 'su' or 'sudo')"
echo "  prefix...............: $prefix"
echo
