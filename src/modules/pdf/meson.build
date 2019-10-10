pdf_src = [ 'etui_module_pdf.c' ]

mod_install_dir = join_paths(etui_package_module, 'pdf', module_arch)

have_pdf = 'no'
if get_option('mupdf') == true and enable_mupdf == true

  mupdf_deps = [
    dependency('freetype2'),
    dependency('harfbuzz'),
    dependency('libjpeg'),
    dependency('libopenjp2'),
    dependency('zlib')
  ]

# test if shared library is available
  mupdf_so = cc.find_library(get_option('mupdf-shared-lib'),
    required : false,
    dirs : get_option('mupdf-path')
  )
  if mupdf_so.found() == true
    mupdf_third_so = cc.find_library(get_option('mupdf-shared-third'),
      required : false,
    dirs : get_option('mupdf-path')
      )
    if mupdf_third_so.found() == true
      have_pdf = 'yes'
      config_h.set('ETUI_BUILD_PDF', 1)
      shared_module('module', pdf_src,
        c_args : [ etui_args, '-DECRIN_ETUI_BUILD' ],
        include_directories : [ config_dir, get_option('mupdf-include-path') ],
        dependencies : [ etui, mupdf_deps, mupdf_so, mupdf_third_so],
        install : true,
        install_dir : mod_install_dir,
        name_suffix : sys_lib_ext
      )
    endif
  else
# test if static library is available
    mupdf_a = cc.find_library(get_option('mupdf-static-lib'),
      required : false,
      static : true,
      dirs : get_option('mupdf-path')
    )
    if mupdf_a.found() == true
      mupdf_third_a = cc.find_library(get_option('mupdf-static-third'),
        required : false,
        static : true,
        dirs : get_option('mupdf-path')
      )
      if mupdf_third_a.found() == true
        have_pdf = 'yes'
        config_h.set('ETUI_BUILD_PDF', 1)
        shared_module('module', pdf_src,
          c_args : [ etui_args, '-DECRIN_ETUI_BUILD' ],
          include_directories : [ config_dir, get_option('mupdf-include-path') ],
          dependencies : [ etui, mupdf_deps, mupdf_a, mupdf_third_a],
          install : true,
          install_dir : mod_install_dir,
          name_suffix : sys_lib_ext
        )
      endif
    endif
  endif

  if have_pdf == 'no'
    message('mupdf needed for PDF module')
  endif
endif
