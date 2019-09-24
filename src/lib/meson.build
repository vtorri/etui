
etui_header_src = [ 'Etui.h' ]

etui_src = [
  'etui_file.c',
  'etui_file.h',
  'etui_main.c',
  'etui_module.c',
  'etui_module.h',
  'etui_private.h',
  'etui_smart.c'
]

etui_lib = library('etui', etui_src,
  c_args : [ etui_args ],
  dependencies : etui_deps,
  include_directories : config_dir,
  install : true,
  version : meson.project_version()
)

etui = declare_dependency(
  include_directories : [ include_directories('.')],
  link_with : etui_lib,
  dependencies : etui_deps
)

install_headers(etui_header_src,
  install_dir : dir_pkginclude + '-' + v_maj
)
