project('etui', 'c',
  version : '0.0.4',
  meson_version : '>= 0.53',
  default_options : [
    'warning_level=2',
    'buildtype=debugoptimized',
    'c_std=c99'
  ]
)

v_array = meson.project_version().split('.')
v_maj = v_array[0]
v_min = v_array[1]
v_mic = v_array[2]

# install paths
dir_prefix = get_option('prefix')
dir_include = join_paths(dir_prefix, get_option('includedir'))
dir_pkginclude = join_paths(dir_include, meson.project_name())
dir_bin = join_paths(dir_prefix, get_option('bindir'))
dir_lib = join_paths(dir_prefix, get_option('libdir'))
dir_data = join_paths(dir_prefix, get_option('datadir'))
dir_pkgdata = join_paths(dir_data, meson.project_name())

# host

host_os = host_machine.system()
linux = ['linux']
bsd = ['bsd', 'freebsd', 'dragonfly', 'netbsd', 'openbsd']
osx = ['darwin']
win32 = ['windows']
cygwin = ['cygwin']
sys_linux = linux.contains(host_os)
sys_bsd = bsd.contains(host_os)
sys_windows = win32.contains(host_os)
sys_cygwin = cygwin.contains(host_os)
sys_osx = osx.contains(host_os)

host_cpu = host_machine.cpu()

module_arch = host_os + '-' + host_cpu + '-' + v_maj + '.' + v_min + '.' + v_mic

if sys_linux==true or sys_bsd==true
  sys_lib_ext = 'so'
elif sys_windows == true or sys_cygwin == true
  sys_lib_ext = 'dll'
elif sys_osx == true
  sys_lib_ext = 'dylib'
else
  error('System ' + host_os + ' not known')
endif

# binaries

cc = meson.get_compiler('c')

etui_cflags = []
etui_cflags_try = [
  '-Wshadow',
  '-Wstrict-prototypes',
  '-Werror=missing-prototypes',
  '-Werror=pointer-arith',
  '-Wno-missing-field-initializers']

foreach cf: etui_cflags_try
  if cc.has_argument(cf) == true
    etui_cflags += cf
  endif
endforeach
add_global_arguments(etui_cflags, language: 'c')

have_visibility_hidden = cc.has_argument('-fvisibility=hidden')
if have_visibility_hidden
  add_global_arguments('-fvisibility=hidden', language: 'c')
endif

pkgconfig = import('pkgconfig')
windows = import('windows')

# libraries

config_dir = [include_directories('.')]

efl_req = '>= 1.20'
etui_deps = [
  dependency('eina', version : efl_req),
  dependency('ecore', version : efl_req),
  dependency('evas', version : efl_req),
  dependency('eio', version : efl_req)
]

if get_option('nls') == true
#  subdir('po')
   etui_deps += cc.find_library('intl', required: false)
endif

etui_args = [ '-DDLL_EXPORT', '-D_POSIX_C_SOURCE=200809L', '-D_XOPEN_SOURCE=500' ]

# configuration

config_h = configuration_data()
config_h.set_quoted('MODULE_ARCH', module_arch)
config_h.set_quoted('PACKAGE_NAME', meson.project_name())
config_h.set_quoted('PACKAGE_VERSION', meson.project_version())
config_h.set_quoted('PACKAGE_BIN_DIR', dir_bin)
config_h.set_quoted('PACKAGE_LIB_DIR', dir_lib)
config_h.set_quoted('PACKAGE_DATA_DIR', dir_data)
config_h.set_quoted('MODULE_EXT', sys_lib_ext)

subdir('src/lib')
subdir('src/modules')
subdir('src/bin')
subdir('data')
subdir('data/themes')

# Use config_h after all subdirs have set values

configure_file(output : 'config.h', configuration : config_h)

pkgconf = configuration_data()

pkgconf.set('prefix', get_option('prefix'))
pkgconf.set('exec_prefix', '${prefix}')
pkgconf.set('libdir', '${prefix}/@0@'.format(get_option('libdir')))
pkgconf.set('includedir', '${prefix}/@0@'.format(get_option('includedir')))
pkgconf.set('pkgincludedir', '${prefix}/@0@'.format(get_option('includedir')) + '/etui')
pkgconf.set('VMAJ', v_maj)
pkgconf.set('VERSION', meson.project_version())
pkgconf.set('requirements_etui_pc', 'eina ecore evas eio')

pkg_install_dir = '@0@/pkgconfig'.format(get_option('libdir'))

configure_file(
  input : join_paths(meson.source_root(), 'etui.pc.in'),
  output : 'etui.pc',
  configuration : pkgconf,
  install_dir : pkg_install_dir
)

# output

if enable_djvu == true
  djvu_msg = have_djvu
else
  djvu_msg = 'no (pass -Dlicense=gplv2 or -Dlicense=agplv3 to enable Djvu module)'
endif

if enable_mupdf == true
  pdf_msg = have_pdf
else
  pdf_msg = 'no (pass -Dlicense=agplv3 to enable PDF module and flags for MuPDF'
endif

summary({'OS': host_os,
         'Comic Book': have_cb,
         'Djvu': djvu_msg,
         'PDF': pdf_msg,
         'Tiff': have_tiff,
        }, section: 'Configuration Options Summary:')

summary({'prefix': dir_prefix,
         'bindir': dir_bin,
         'libdir': dir_lib,
         'incdir': dir_include,
         'pkgincdir': dir_pkginclude,
         'datadir': dir_data,
         'pkgdatadir': dir_pkgdata,
        }, section: 'Directories:')

summary({'compilation': 'ninja',
         'installation': 'ninja install',
        }, section: 'Compilation')
