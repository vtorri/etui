
etui_package_module = join_paths(dir_lib, 'etui', 'modules')

enable_djvu = false
enable_mupdf = false
license = get_option('license')
if license == 'lgplv2'
  have_license = 'LPGL 2.1'
elif license == 'gplv2'
  have_license = 'GPL 2.1'
  enable_djvu = true
elif license == 'agplv3'
  have_license = 'AGPL 3'
  enable_djvu = true
  enable_mupdf = true
else
  error('license option invalid')
endif

subdir('cb')
subdir('djvu')
subdir('pdf')
subdir('tiff')
