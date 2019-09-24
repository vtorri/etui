cb_src = [ 'etui_module_cb.c' ]

mod_install_dir = join_paths(etui_package_module, 'cb', module_arch)

have_cb = 'no'
if get_option('cb') == true
  cb_deps = dependency('libarchive', version : ' >= 3', required : false)
  if cb_deps.found()
    have_cb = 'yes'
    config_h.set('ETUI_BUILD_CB', 1)
    shared_module('module', cb_src,
      c_arg : etui_args,
      include_directories : config_dir,
      dependencies : [ etui, cb_deps],
      install : true,
      install_dir : mod_install_dir,
      name_suffix : sys_lib_ext
    )
  else
    message('libarchive >= 3 needed for ComicBook module')
  endif
endif
