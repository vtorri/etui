tiff_src = [ 'etui_module_tiff.c' ]

mod_install_dir = join_paths(etui_package_module, 'tiff', module_arch)

have_tiff = 'no'
if get_option('tiff') == true
  tiff_deps = dependency('libtiff-4', required : false)
  if tiff_deps.found()
    have_tiff = 'yes'
    config_h.set('ETUI_BUILD_TIFF', 1)
    shared_module('module', tiff_src,
      c_args : [ etui_args ],
      include_directories : config_dir,
      dependencies : [ etui, tiff_deps],
      install : true,
      install_dir : mod_install_dir,
      name_suffix : sys_lib_ext
    )
  else
    message('tiff library needed for Tiff module')
  endif
endif
