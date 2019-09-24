
etui_bin_src = [
  'etui_config.c',
  'etui_config.h',
  'etui_controls.c',
  'etui_controls.h',
#  'etui_doc_genlist.c',
#  'etui_doc_genlist.h',
  'etui_doc_simple.c',
  'etui_doc_simple.h',
  'etui_main.c',
  'etui_main.h',
  'etui_open.c',
  'etui_open.h',
  'etui_private.h',
  'etui_settings.c',
  'etui_settings.h',
  'etui_settings_behavior.c',
  'etui_settings_elm.c',
  'etui_theme.c',
  'etui_theme.h',
  'etui_win.c',
  'etui_win.h'
]

etui_bin_args = [ '-DHAVE_CONFIG_H' ]

if sys_windows == true
  rcconf = configuration_data()
  rcconf.set('SOURCEDIR', meson.source_root())
  rcconf.set('VERSION', meson.project_version())
  rcconf.set('VMAJ', v_maj)
  rcconf.set('VMIN', v_min)
  rcconf.set('VMIC', v_mic)
  etui_rc = configure_file(
    input : 'etui.rc.in',
    output : 'etui.rc',
    configuration : rcconf
  )
  etui_rc_res = windows.compile_resources(etui_rc)
  etui_bin_src += [ etui_rc_res ]
endif

etui_bin = executable('etui', etui_bin_src,
  c_args : [ etui_bin_args ],
  dependencies : [ etui,  dependency('elementary', version : efl_req) ],
  include_directories : config_dir,
  install : true
)
