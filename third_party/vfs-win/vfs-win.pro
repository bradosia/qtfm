QT += widgets concurrent

TARGET = vfs-win
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    src/vfs-win.cpp

HEADERS += \
    src/vfs-win.hpp

DESTDIR = $${top_builddir}/lib$${LIBSUFFIX}
