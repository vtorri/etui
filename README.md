[![Build Status](https://travis-ci.org/vtorri/etui.svg?branch=master)](https://travis-ci.org/vtorri/etui)
<a href="https://scan.coverity.com/projects/vtorri-etui">
<img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/13637/badge.svg"/></a>

### Étui
##### A Multiple Document Library and Viewer

### License:

This library is released under the GPL v3 license

### Library:


**Requirements** :

  - Evil (on Windows)
  - Eina
  - Eet
  - Ecore
  - Evas
  - Eio

### Binary:

 **Requirements**:
  - Evil (Windows)
  - Eina
  - Eet
  - Evas
  - Ecore
  - Ecore_Input
  - Elementary

## Supported backends (Current and Future):

- [x] **PDF** and **XPS**: Requires *zlib, freetype2, libopenjp2, muPDF 1.12*.
- [x] **COMIC BOOK** (cbz, cbr, cb7, cbt, cba): Requires *libarchive*.
- [x] **DjVu**: Requires *libdjvu (DjVuLibre)*
- [x] **TIFF**: Requires *libtiff*.
- [ ] **Postscript**
	- ***Requirements***: *libgs*
	- ***compilation***: *make so (debug mode: make sodebug)*
	- ***installation***: *make soinstall (debug mode: make sodebuginstall)*
- [ ] **DVI**: Requires *libdvi*
- [ ] **EPUB**: Requires *libarchive*
- [ ] **CHM**: Requires *libarchive*

## Compilation:

Common usage of meson. Here are some example to compile with PDF and DJVU support:

- **Windows** (MSYS2 + mingw-w64):

prerequisites: install [Ewpi](https://github.com/vtorri/ewpi) and then the EFL. If ewpi is installed in /opt/ewpi:

```
mkdir builddir && cd builddir
meson .. --default-library shared --prefix=$HOME/etui \
    -Dlicense=agplv3 \
    -Dmupdf-libdir=/opt/ewpi/lib \
    -Dmupdf-includedir=/opt/ewpi/include
```

- **Ubuntu**:

prerequisites:

`
apt-get install libmupdf-dev libarchive-dev libjbig2dec0-dev libopenjp2-7-dev libfreetype6-dev libharfbuzz-dev libjpeg-dev zlib1g-dev libtiff-dev libdjvulibre-dev
`

compilation:

```
mkdir builddir && cd builddir
meson .. --default-library shared --prefix=$HOME/etui -Dlicense=agplv3
```

- **Fedora**:

prerequisites:

`
dnf install mupdf-devel libarchive-devel jbig2dec-devel openjpeg2-devel freetype-devel harfbuzz-devel libjpeg-devel zlib-devel libtiff-devel djvulibre-devel
`

compilation:

```
mkdir builddir && cd builddir
meson .. --default-library shared --prefix=$HOME/etui -Dlicense=agplv3
```

- **ArchLinux**: *See pkgbuild/PKGBUILD*

- **Gentoo**:

`
./configure --enable-gpl --with-mupdf-shared-libs="-lmupdf"
`

- **OpenBSD** or **FreeBsd**:

`
./configure --enable-gpl --with-mupdf-includedir=-I/usr/local/include --with-mupdf-shared-libs="-L/usr/local/lib -lmupdf -lmupdfthird"
`

## Key bindings: ([C] means can be configured)

- **Ctrl + Q**: Quit
- **Ctrl + O**: Open File
- **[C] F1**: Help
- **[C] Right Arrow** or **Space** : Next page
- **[C] Left Arrow** or **Backspace** : Previous Page
- **[C] Page Up** : Page + 10
- **[C] Page Down** : Page - 10
- **[C] F11**: Toggle Fullscreen Esc: Exit Fullscreen
- **[C] Ctrl + +** : Scale * 1.2
- **[C] Ctrl + -** : Scale / 1.2
- ***[C] Ctrl + 0** : Zoom to Fit Height (not done yet)*
- **[C] Ctrl + 1** : Zoom to Original Size
- ***[C] Ctrl + 2** : Zoom to Fit Width (not done yet)*
- **[C] Ctrl + Home** : Go To First Page
- **[C] Ctrl + End** : Go To Last Page

## Mouse bindings:

- **Double Left Click** : Toggle Fullscreen
- **Ctrl + Wheel Up** : Scale * 1.2
- **Ctrl - Wheel Down** : Scale / 1.2
