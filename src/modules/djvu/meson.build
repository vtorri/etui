djvu_src = [ 'etui_module_djvu.c' ]

mod_install_dir = join_paths(etui_package_module, 'djvu', module_arch)

have_djvu = 'no'
if get_option('djvu') == true and enable_djvu == true
  djvu_deps = dependency('ddjvuapi', required : false)
  if djvu_deps.found()
    have_djvu = 'yes'
    config_h.set('ETUI_BUILD_DJVU', 1)
    shared_module('module', djvu_src,
      c_args : [ etui_args, '-DECRIN_ETUI_BUILD' ],
      include_directories : config_dir,
      dependencies : [ etui, djvu_deps],
      install : true,
      install_dir : mod_install_dir,
      name_suffix : sys_lib_ext
    )
  else
    message('ddjvuapi needed for DJvu module')
  endif
endif
