
# icons

sizes = [
  '16x16',
  '22x22',
  '24x24',
  '32x32',
  '36x36',
  '48x48',
  '64x64',
  '72x72',
  '96x96',
  '128x128',
  '192x192',
  '256x256',
  '384x384',
  '512x512'
]

foreach sz : sizes
  install_data(join_paths('icons', sz) + '/etui.png',
    install_dir : join_paths(get_option('datadir'), 'icons/hicolor', sz, 'apps')
  )
endforeach

install_data('icons/scalable/etui.svg',
  install_dir : join_paths(get_option('datadir'), 'icons/hicolor/scalable/apps')
)

# desktop

install_data('desktop/etui.desktop',
  install_dir : join_paths(get_option('datadir'), 'desktop')
)
