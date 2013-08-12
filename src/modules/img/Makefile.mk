
if ETUI_BUILD_STATIC_IMG

src_lib_libetui_la_SOURCES += \
src/modules/img/etui_module_img.c \
src/modules/img/etui_module_img.h

src_lib_libetui_la_CPPFLAGS += \
@IMG_CFLAGS@

src_lib_libetui_la_LIBADD += \
@IMG_LIBS@

else

etui_modules_imgdir = $(pkglibdir)/modules/img/$(MODULE_ARCH)
etui_modules_img_LTLIBRARIES = src/modules/img/module.la

src_modules_img_module_la_SOURCES = \
src/modules/img/etui_module_img.c \
src/modules/img/etui_module_img.h

# unrar
src_modules_img_module_la_SOURCES += \
src/modules/img/unrar/archive.cpp \
src/modules/img/unrar/arcread.cpp \
src/modules/img/unrar/blake2s.cpp \
src/modules/img/unrar/cmddata.cpp \
src/modules/img/unrar/consio.cpp \
src/modules/img/unrar/crc.cpp \
src/modules/img/unrar/crypt.cpp \
src/modules/img/unrar/dll.cpp \
src/modules/img/unrar/encname.cpp \
src/modules/img/unrar/errhnd.cpp \
src/modules/img/unrar/extinfo.cpp \
src/modules/img/unrar/extract.cpp \
src/modules/img/unrar/filcreat.cpp \
src/modules/img/unrar/file.cpp \
src/modules/img/unrar/filefn.cpp \
src/modules/img/unrar/filestr.cpp \
src/modules/img/unrar/find.cpp \
src/modules/img/unrar/getbits.cpp \
src/modules/img/unrar/global.cpp \
src/modules/img/unrar/hash.cpp \
src/modules/img/unrar/headers.cpp \
src/modules/img/unrar/isnt.cpp \
src/modules/img/unrar/match.cpp \
src/modules/img/unrar/options.cpp \
src/modules/img/unrar/pathfn.cpp \
src/modules/img/unrar/qopen.cpp \
src/modules/img/unrar/rar.cpp \
src/modules/img/unrar/rarpch.cpp \
src/modules/img/unrar/rarvm.cpp \
src/modules/img/unrar/rawread.cpp \
src/modules/img/unrar/rdwrfn.cpp \
src/modules/img/unrar/recvol.cpp \
src/modules/img/unrar/rijndael.cpp \
src/modules/img/unrar/rs.cpp \
src/modules/img/unrar/rs16.cpp \
src/modules/img/unrar/scantree.cpp \
src/modules/img/unrar/secpassword.cpp \
src/modules/img/unrar/sha1.cpp \
src/modules/img/unrar/sha256.cpp \
src/modules/img/unrar/smallfn.cpp \
src/modules/img/unrar/strfn.cpp \
src/modules/img/unrar/strlist.cpp \
src/modules/img/unrar/system.cpp \
src/modules/img/unrar/threadpool.cpp \
src/modules/img/unrar/timefn.cpp \
src/modules/img/unrar/unicode.cpp \
src/modules/img/unrar/unpack.cpp \
src/modules/img/unrar/volume.cpp \
src/modules/img/unrar/archive.hpp \
src/modules/img/unrar/array.hpp \
src/modules/img/unrar/blake2s.hpp \
src/modules/img/unrar/cmddata.hpp \
src/modules/img/unrar/coder.hpp \
src/modules/img/unrar/compress.hpp \
src/modules/img/unrar/consio.hpp \
src/modules/img/unrar/crc.hpp \
src/modules/img/unrar/crypt.hpp \
src/modules/img/unrar/dll.hpp \
src/modules/img/unrar/encname.hpp \
src/modules/img/unrar/errhnd.hpp \
src/modules/img/unrar/extinfo.hpp \
src/modules/img/unrar/extract.hpp \
src/modules/img/unrar/filcreat.hpp \
src/modules/img/unrar/file.hpp \
src/modules/img/unrar/filefn.hpp \
src/modules/img/unrar/filestr.hpp \
src/modules/img/unrar/find.hpp \
src/modules/img/unrar/getbits.hpp \
src/modules/img/unrar/global.hpp \
src/modules/img/unrar/hash.hpp \
src/modules/img/unrar/headers.hpp \
src/modules/img/unrar/headers5.hpp \
src/modules/img/unrar/isnt.hpp \
src/modules/img/unrar/list.hpp \
src/modules/img/unrar/loclang.hpp \
src/modules/img/unrar/log.hpp \
src/modules/img/unrar/match.hpp \
src/modules/img/unrar/model.hpp \
src/modules/img/unrar/options.hpp \
src/modules/img/unrar/os.hpp \
src/modules/img/unrar/pathfn.hpp \
src/modules/img/unrar/qopen.hpp \
src/modules/img/unrar/rar.hpp \
src/modules/img/unrar/rardefs.hpp \
src/modules/img/unrar/rarlang.hpp \
src/modules/img/unrar/raros.hpp \
src/modules/img/unrar/rartypes.hpp \
src/modules/img/unrar/rarvm.hpp \
src/modules/img/unrar/rawread.hpp \
src/modules/img/unrar/rdwrfn.hpp \
src/modules/img/unrar/recvol.hpp \
src/modules/img/unrar/resource.hpp \
src/modules/img/unrar/rijndael.hpp \
src/modules/img/unrar/rs.hpp \
src/modules/img/unrar/rs16.hpp \
src/modules/img/unrar/savepos.hpp \
src/modules/img/unrar/scantree.hpp \
src/modules/img/unrar/secpassword.hpp \
src/modules/img/unrar/sha1.hpp \
src/modules/img/unrar/sha256.hpp \
src/modules/img/unrar/smallfn.hpp \
src/modules/img/unrar/strfn.hpp \
src/modules/img/unrar/strlist.hpp \
src/modules/img/unrar/suballoc.hpp \
src/modules/img/unrar/system.hpp \
src/modules/img/unrar/threadpool.hpp \
src/modules/img/unrar/timefn.hpp \
src/modules/img/unrar/unicode.hpp \
src/modules/img/unrar/unpack.hpp \
src/modules/img/unrar/version.hpp \
src/modules/img/unrar/volume.hpp

src_modules_img_module_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-DECRIN_ETUI_BUILD \
@ETUI_CFLAGS@ \
@IMG_CFLAGS@

# unrar

src_modules_img_module_la_CPPFLAGS += \
-DRAR_SMP \
-DRARDLL \
-DUNRAR

src_modules_img_module_la_CFLAGS = \
@ETUI_LIB_CFLAGS@

src_modules_img_module_la_CXXFLAGS = \
@ETUI_LIB_CXXFLAGS@

src_modules_img_module_la_LIBADD = \
src/lib/libetui.la \
@ETUI_LIBS@ \
@IMG_LIBS@

src_modules_img_module_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_img_module_la_LIBTOOLFLAGS = --tag=disable-static

endif

EXTRA_DIST += \
src/modules/img/unrar/arccmt.cpp \
src/modules/img/unrar/blake2s_sse.cpp \
src/modules/img/unrar/blake2sp.cpp \
src/modules/img/unrar/coder.cpp \
src/modules/img/unrar/crypt1.cpp \
src/modules/img/unrar/crypt2.cpp \
src/modules/img/unrar/crypt3.cpp \
src/modules/img/unrar/crypt5.cpp \
src/modules/img/unrar/hardlinks.cpp \
src/modules/img/unrar/log.cpp \
src/modules/img/unrar/model.cpp \
src/modules/img/unrar/rarvmtbl.cpp \
src/modules/img/unrar/recvol3.cpp \
src/modules/img/unrar/recvol5.cpp \
src/modules/img/unrar/suballoc.cpp \
src/modules/img/unrar/threadmisc.cpp \
src/modules/img/unrar/ulinks.cpp \
src/modules/img/unrar/unpack15.cpp \
src/modules/img/unrar/unpack20.cpp \
src/modules/img/unrar/unpack30.cpp \
src/modules/img/unrar/unpack50.cpp \
src/modules/img/unrar/unpack50frag.cpp \
src/modules/img/unrar/unpack50mt.cpp \
src/modules/img/unrar/unpackinline.cpp \
src/modules/img/unrar/uowners.cpp \
src/modules/img/unrar/win32acl.cpp \
src/modules/img/unrar/win32lnk.cpp \
src/modules/img/unrar/win32stm.cpp
